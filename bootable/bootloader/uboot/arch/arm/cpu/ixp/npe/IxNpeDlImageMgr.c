


#include "IxOsal.h"

#include "IxNpeDlImageMgr_p.h"
#include "IxNpeDlMacros_p.h"

#define IX_NPE_MICROCODE_FIRMWARE_INCLUDED 1
#include "IxNpeMicrocode.h"

#define NPE_IMAGE_MARKER 0xfeedf00d


typedef struct
{
    UINT32 size;
    UINT32 offset;
    UINT32 id;
} IxNpeDlImageMgrImageEntry;

typedef union
{
    IxNpeDlImageMgrImageEntry image;
    UINT32 eohMarker;
} IxNpeDlImageMgrHeaderEntry;

typedef struct
{
    UINT32 signature;
    /* 1st entry in the header (there may be more than one) */
    IxNpeDlImageMgrHeaderEntry entry[1];
} IxNpeDlImageMgrImageLibraryHeader;


typedef struct
{
    UINT32 marker;
    UINT32 id;
    UINT32 size;
} IxNpeDlImageMgrImageHeader;

/* module statistics counters */
typedef struct
{
    UINT32 invalidSignature;
    UINT32 imageIdListOverflow;
    UINT32 imageIdNotFound;
} IxNpeDlImageMgrStats;


static IxNpeDlImageMgrStats ixNpeDlImageMgrStats;

static UINT32* getIxNpeMicroCodeImageLibrary(void)
{
	char *s;

	if ((s = getenv("npe_ucode")) != NULL)
		return (UINT32*) simple_strtoul(s, NULL, 16);
	else
		return NULL;
}

PRIVATE BOOL
ixNpeDlImageMgrSignatureCheck (UINT32 *microCodeImageLibrary);

PRIVATE void  
ixNpeDlImageMgrImageIdFormat (UINT32 rawImageId, IxNpeDlImageId *imageId);

PRIVATE BOOL
ixNpeDlImageMgrImageIdCompare (IxNpeDlImageId *imageIdA, 
				 IxNpeDlImageId *imageIdB);
				 
PRIVATE BOOL
ixNpeDlImageMgrNpeFunctionIdCompare (IxNpeDlImageId *imageIdA,
				       IxNpeDlImageId *imageIdB);

#if 0
PRIVATE IX_STATUS
ixNpeDlImageMgrImageFind_legacy (UINT32 *imageLibrary,
                                 UINT32 imageId,
                                 UINT32 **imagePtr,
                                 UINT32 *imageSize);

IX_STATUS
ixNpeDlImageMgrMicrocodeImageLibraryOverride (
    UINT32 *clientImageLibrary)
{
    IX_STATUS status = IX_SUCCESS;

    IX_NPEDL_TRACE0 (IX_NPEDL_FN_ENTRY_EXIT, 
		     "Entering ixNpeDlImageMgrMicrocodeImageLibraryOverride\n");

    if (ixNpeDlImageMgrSignatureCheck (clientImageLibrary))
    {
	IxNpeMicroCodeImageLibrary = clientImageLibrary;
    }
    else
    {
	IX_NPEDL_ERROR_REPORT ("ixNpeDlImageMgrMicrocodeImageLibraryOverride: "
			       "Client-supplied image has invalid signature\n");
	status = IX_FAIL;
    }

    IX_NPEDL_TRACE1 (IX_NPEDL_FN_ENTRY_EXIT, 
		     "Exiting ixNpeDlImageMgrMicrocodeImageLibraryOverride: status = %d\n",
		     status);
    return status;
}
#endif

IX_STATUS
ixNpeDlImageMgrImageListExtract (
    IxNpeDlImageId *imageListPtr,
    UINT32 *numImages)
{
    UINT32 rawImageId;
    IxNpeDlImageId formattedImageId;
    IX_STATUS status = IX_SUCCESS;
    UINT32 imageCount = 0;
    IxNpeDlImageMgrImageLibraryHeader *header;

    IX_NPEDL_TRACE0 (IX_NPEDL_FN_ENTRY_EXIT, 
		     "Entering ixNpeDlImageMgrImageListExtract\n");

    header = (IxNpeDlImageMgrImageLibraryHeader *) getIxNpeMicroCodeImageLibrary();

    if (ixNpeDlImageMgrSignatureCheck (getIxNpeMicroCodeImageLibrary()))
    {
	/* for each image entry in the image header ... */
	while (header->entry[imageCount].eohMarker !=
	       IX_NPEDL_IMAGEMGR_END_OF_HEADER)
	{
	    /*
	     * if the image list container from calling function has capacity,
	     * add the image id to the list 
	     */
	    if ((imageListPtr != NULL) && (imageCount < *numImages))
	    {
		rawImageId = header->entry[imageCount].image.id;
	        ixNpeDlImageMgrImageIdFormat (rawImageId, &formattedImageId);
		imageListPtr[imageCount] = formattedImageId;
	    }
	    /* imageCount reflects no. of image entries in image library header */
	    imageCount++;  
	}
	
	/*
	 * if image list container from calling function was too small to
	 * contain all image ids in the header, set return status to FAIL
	 */
	if ((imageListPtr != NULL) && (imageCount > *numImages))
	{
	    status = IX_FAIL;
	    IX_NPEDL_ERROR_REPORT ("ixNpeDlImageMgrImageListExtract: "
				   "number of Ids found exceeds list capacity\n");
	    ixNpeDlImageMgrStats.imageIdListOverflow++;
	}
	/* return number of image ids found in image library header */
	*numImages = imageCount;  
    }
    else
    {
	status = IX_FAIL;
	IX_NPEDL_ERROR_REPORT ("ixNpeDlImageMgrImageListExtract: "
			       "invalid signature in image\n");
    }
    
    IX_NPEDL_TRACE1 (IX_NPEDL_FN_ENTRY_EXIT, 
		     "Exiting ixNpeDlImageMgrImageListExtract: status = %d\n",
		     status);
    return status;
}


IX_STATUS
ixNpeDlImageMgrImageLocate (
    IxNpeDlImageId *imageId,
    UINT32 **imagePtr,
    UINT32 *imageSize)
{
    UINT32 imageOffset;
    UINT32 rawImageId;
    IxNpeDlImageId formattedImageId;
    /* used to index image entries in image library header */
    UINT32 imageCount = 0;   
    IX_STATUS status = IX_FAIL;
    IxNpeDlImageMgrImageLibraryHeader *header;

    IX_NPEDL_TRACE0 (IX_NPEDL_FN_ENTRY_EXIT,
		     "Entering ixNpeDlImageMgrImageLocate\n");

    header = (IxNpeDlImageMgrImageLibraryHeader *) getIxNpeMicroCodeImageLibrary();

    if (ixNpeDlImageMgrSignatureCheck (getIxNpeMicroCodeImageLibrary()))
    {
	/* for each image entry in the image library header ... */
	while (header->entry[imageCount].eohMarker !=
	       IX_NPEDL_IMAGEMGR_END_OF_HEADER)
	{
	    rawImageId = header->entry[imageCount].image.id;
	    ixNpeDlImageMgrImageIdFormat (rawImageId, &formattedImageId);
	    /* if a match for imageId is found in the image library header... */
	    if (ixNpeDlImageMgrImageIdCompare (imageId, &formattedImageId))
	    {
		/*
		 * get pointer to the image in the image library using offset from
		 * 1st word in image library
		 */
		UINT32 *tmp=getIxNpeMicroCodeImageLibrary();
		imageOffset = header->entry[imageCount].image.offset;
		*imagePtr = &tmp[imageOffset];
		/* get the image size */
		*imageSize = header->entry[imageCount].image.size;
		status = IX_SUCCESS;
		break;
	    }
	    imageCount++;
	}
	if (status != IX_SUCCESS)
	{
	    IX_NPEDL_ERROR_REPORT ("ixNpeDlImageMgrImageLocate: "
				   "imageId not found in image library header\n");
	    ixNpeDlImageMgrStats.imageIdNotFound++;
	}
    }
    else
    {
	IX_NPEDL_ERROR_REPORT ("ixNpeDlImageMgrImageLocate: "
			       "invalid signature in image library\n");
    }

    IX_NPEDL_TRACE1 (IX_NPEDL_FN_ENTRY_EXIT,
		     "Exiting ixNpeDlImageMgrImageLocate: status = %d\n", status);
    return status;
}

IX_STATUS
ixNpeDlImageMgrLatestImageExtract (IxNpeDlImageId *imageId)
{
    UINT32 imageCount = 0; 
    UINT32 rawImageId;
    IxNpeDlImageId formattedImageId;
    IX_STATUS status = IX_FAIL;
    IxNpeDlImageMgrImageLibraryHeader *header;
    
    
    IX_NPEDL_TRACE0 (IX_NPEDL_FN_ENTRY_EXIT,
		     "Entering ixNpeDlImageMgrLatestImageExtract\n");
		     
    header = (IxNpeDlImageMgrImageLibraryHeader *) getIxNpeMicroCodeImageLibrary();
    
    if (ixNpeDlImageMgrSignatureCheck (getIxNpeMicroCodeImageLibrary()))
    {
	/* for each image entry in the image library header ... */
	while (header->entry[imageCount].eohMarker !=
	       IX_NPEDL_IMAGEMGR_END_OF_HEADER)
	{
	    rawImageId = header->entry[imageCount].image.id;
	    ixNpeDlImageMgrImageIdFormat (rawImageId, &formattedImageId);
	    /* 
	     * if a match for the npe Id and functionality Id of the imageId is
	     *  found in the image library header... 
	     */
            if(ixNpeDlImageMgrNpeFunctionIdCompare(imageId, &formattedImageId))
            {
                if(imageId->major <= formattedImageId.major)
                {
                    if(imageId->minor < formattedImageId.minor)
                    {
                        imageId->minor = formattedImageId.minor;
                    }
                    imageId->major = formattedImageId.major;
                }
                status = IX_SUCCESS;
            }
            imageCount++;
	}
	if (status != IX_SUCCESS)
	{
	    IX_NPEDL_ERROR_REPORT ("ixNpeDlImageMgrLatestImageExtract: "
				   "imageId not found in image library header\n");
	    ixNpeDlImageMgrStats.imageIdNotFound++;
	}
    }
    else
    {
	IX_NPEDL_ERROR_REPORT ("ixNpeDlImageMgrLatestImageGet: "
			       "invalid signature in image library\n");
    }

    IX_NPEDL_TRACE1 (IX_NPEDL_FN_ENTRY_EXIT,
		     "Exiting ixNpeDlImageMgrLatestImageGet: status = %d\n", status);
    return status;
}

PRIVATE BOOL
ixNpeDlImageMgrSignatureCheck (UINT32 *microCodeImageLibrary)
{
    IxNpeDlImageMgrImageLibraryHeader *header =
	(IxNpeDlImageMgrImageLibraryHeader *) microCodeImageLibrary;
    BOOL result = TRUE;

    if (!header || header->signature != IX_NPEDL_IMAGEMGR_SIGNATURE)
    {
	result = FALSE;
	ixNpeDlImageMgrStats.invalidSignature++;
    }

    return result;
}


PRIVATE void
ixNpeDlImageMgrImageIdFormat (
    UINT32 rawImageId, 
    IxNpeDlImageId *imageId)
{  
    imageId->npeId = (rawImageId >>
				IX_NPEDL_IMAGEID_NPEID_OFFSET) &
	IX_NPEDL_NPEIMAGE_FIELD_MASK;
    imageId->functionalityId = (rawImageId >> 
				  IX_NPEDL_IMAGEID_FUNCTIONID_OFFSET) &
	IX_NPEDL_NPEIMAGE_FIELD_MASK;
    imageId->major = (rawImageId >>
				IX_NPEDL_IMAGEID_MAJOR_OFFSET) &
	IX_NPEDL_NPEIMAGE_FIELD_MASK;
    imageId->minor = (rawImageId >>
				IX_NPEDL_IMAGEID_MINOR_OFFSET) &
	IX_NPEDL_NPEIMAGE_FIELD_MASK;

}


PRIVATE BOOL
ixNpeDlImageMgrImageIdCompare (
    IxNpeDlImageId *imageIdA,
    IxNpeDlImageId *imageIdB)
{
    if ((imageIdA->npeId   == imageIdB->npeId)   &&
	(imageIdA->functionalityId == imageIdB->functionalityId) &&
	(imageIdA->major   == imageIdB->major)   &&
	(imageIdA->minor   == imageIdB->minor))
    {
	return TRUE;
    }
    else
    {
	return FALSE;
    }
}

PRIVATE BOOL
ixNpeDlImageMgrNpeFunctionIdCompare (
    IxNpeDlImageId *imageIdA,
    IxNpeDlImageId *imageIdB)
{
    if ((imageIdA->npeId   == imageIdB->npeId)   &&
	(imageIdA->functionalityId == imageIdB->functionalityId))
    {
	return TRUE;
    }
    else
    {
	return FALSE;
    }
}


void
ixNpeDlImageMgrStatsShow (void)
{
    ixOsalLog (IX_OSAL_LOG_LVL_USER,
               IX_OSAL_LOG_DEV_STDOUT,
               "\nixNpeDlImageMgrStatsShow:\n"
               "\tInvalid Image Signatures: %u\n"
               "\tImage Id List capacity too small: %u\n"
               "\tImage Id not found: %u\n\n",
               ixNpeDlImageMgrStats.invalidSignature,
               ixNpeDlImageMgrStats.imageIdListOverflow,
               ixNpeDlImageMgrStats.imageIdNotFound,
               0,0,0);
}


void
ixNpeDlImageMgrStatsReset (void)
{
    ixNpeDlImageMgrStats.invalidSignature = 0;
    ixNpeDlImageMgrStats.imageIdListOverflow = 0;
    ixNpeDlImageMgrStats.imageIdNotFound = 0;
}


#if 0
PRIVATE IX_STATUS
ixNpeDlImageMgrImageFind_legacy (
    UINT32 *imageLibrary,
    UINT32 imageId,
    UINT32 **imagePtr,
    UINT32 *imageSize)
{
    UINT32 imageOffset;
    /* used to index image entries in image library header */
    UINT32 imageCount = 0;   
    IX_STATUS status = IX_FAIL;
    IxNpeDlImageMgrImageLibraryHeader *header;
    BOOL imageFound = FALSE;

    IX_NPEDL_TRACE0 (IX_NPEDL_FN_ENTRY_EXIT,
		     "Entering ixNpeDlImageMgrImageFind\n");


    /* If user didn't specify a library to use, use the default
     * one from IxNpeMicrocode.h
     */
    if (imageLibrary == NULL)
    {
	imageLibrary = IxNpeMicroCodeImageLibrary;
    }
    
    if (ixNpeDlImageMgrSignatureCheck (imageLibrary))
    {
	header = (IxNpeDlImageMgrImageLibraryHeader *) imageLibrary;
    
	/* for each image entry in the image library header ... */
	while ((header->entry[imageCount].eohMarker !=
               IX_NPEDL_IMAGEMGR_END_OF_HEADER) && !(imageFound))
	{
	    /* if a match for imageId is found in the image library header... */
	    if (imageId == header->entry[imageCount].image.id)
	    {
		/*
		 * get pointer to the image in the image library using offset from
		 * 1st word in image library
		 */
		imageOffset = header->entry[imageCount].image.offset;
		*imagePtr = &imageLibrary[imageOffset];
		/* get the image size */
		*imageSize = header->entry[imageCount].image.size;
		status = IX_SUCCESS;
		imageFound = TRUE;
	    }
	    imageCount++;
	}
	if (status != IX_SUCCESS)
	{
	    IX_NPEDL_ERROR_REPORT ("ixNpeDlImageMgrImageFind: "
				   "imageId not found in image library header\n");
	    ixNpeDlImageMgrStats.imageIdNotFound++;
	}
    }
    else
    {
	IX_NPEDL_ERROR_REPORT ("ixNpeDlImageMgrImageFind: "
			       "invalid signature in image library\n");
    }

    IX_NPEDL_TRACE1 (IX_NPEDL_FN_ENTRY_EXIT,
		     "Exiting ixNpeDlImageMgrImageFind: status = %d\n", status);
    return status;
}
#endif

IX_STATUS
ixNpeDlImageMgrImageFind (
    UINT32 *imageLibrary,
    UINT32 imageId,
    UINT32 **imagePtr,
    UINT32 *imageSize)
{
    IxNpeDlImageMgrImageHeader *image;
    UINT32 offset = 0;

    /* If user didn't specify a library to use, use the default
     * one from IxNpeMicrocode.h
     */
    if (imageLibrary == NULL)
    {
#ifdef IX_NPEDL_READ_MICROCODE_FROM_FILE
	if (ixNpeMicrocode_binaryArray == NULL)
        {
	    printk (KERN_ERR "ixp400.o:  ERROR, no Microcode found in memory\n");
	    return IX_FAIL;
	}
	else
	{
	    imageLibrary = ixNpeMicrocode_binaryArray;
	}
#else
	imageLibrary = getIxNpeMicroCodeImageLibrary();
	if (imageLibrary == NULL)
        {
	    printf ("npe:  ERROR, no Microcode found in memory\n");
	    return IX_FAIL;
	}
#endif /* IX_NPEDL_READ_MICROCODE_FROM_FILE */
    }

#if 0
    /* For backward's compatibility with previous image format */
    if (ixNpeDlImageMgrSignatureCheck(imageLibrary))
    {
        return ixNpeDlImageMgrImageFind_legacy(imageLibrary,
                                               imageId,
                                               imagePtr,
                                               imageSize);
    }
#endif

    while (*(imageLibrary+offset) == NPE_IMAGE_MARKER)
    {
        image = (IxNpeDlImageMgrImageHeader *)(imageLibrary+offset);
        offset += sizeof(IxNpeDlImageMgrImageHeader)/sizeof(UINT32);
        
        if (image->id == imageId)
        {
            *imagePtr = imageLibrary + offset;
            *imageSize = image->size;
            return IX_SUCCESS;
        }
        /* 2 consecutive NPE_IMAGE_MARKER's indicates end of library */
        else if (image->id == NPE_IMAGE_MARKER)
        {
	    IX_NPEDL_ERROR_REPORT ("ixNpeDlImageMgrImageFind: "
				   "imageId not found in image library header\n");
	    ixNpeDlImageMgrStats.imageIdNotFound++;
            /* reached end of library, image not found */
            return IX_FAIL;
        }
        offset += image->size;
    }

    /* If we get here, our image library may be corrupted */
    IX_NPEDL_ERROR_REPORT ("ixNpeDlImageMgrImageFind: "
                           "image library format may be invalid or corrupted\n");
    return IX_FAIL;
}
