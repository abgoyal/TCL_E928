include $(TOPDIR)/config.mk

#CFLAGS += -DDEBUG

LIB	= $(obj)libhwmon.a

COBJS-$(CONFIG_DTT_ADM1021) += adm1021.o
COBJS-$(CONFIG_DTT_ADT7460) += adt7460.o
COBJS-$(CONFIG_DTT_DS1621) += ds1621.o
COBJS-$(CONFIG_DTT_DS1722) += ds1722.o
COBJS-$(CONFIG_DTT_DS1775) += ds1775.o
COBJS-$(CONFIG_DTT_LM63) += lm63.o
COBJS-$(CONFIG_DTT_LM73) += lm73.o
COBJS-$(CONFIG_DTT_LM75) += lm75.o
COBJS-$(CONFIG_DTT_LM81) += lm81.o

COBJS	:= $(COBJS-y)
SRCS	:= $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS))

all:	$(LIB)

$(LIB):	$(obj).depend $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
