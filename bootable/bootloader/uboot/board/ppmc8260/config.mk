TEXT_BASE = 0xfe000000
TEXT_END  = 0xfe080000
PLATFORM_CPPFLAGS += -DTEXT_BASE=$(TEXT_BASE) -I$(TOPDIR)/board