LIB_DIR     = $(REP_DIR)/src/lib/wifi
LIB_INC_DIR = $(LIB_DIR)/include

SHARED_LIB = yes

# FIXME should we *really* add dde_kit to this shared library?
LIBS  += dde_kit libc-setjmp libc

LD_OPT += --version-script=$(LIB_DIR)/symbol.map

SRC_CC += dummies.cc init.cc lxcc_emul.cc pci_driver.cc timer.cc irq.cc \
          work.cc nic.cc socket_call.cc scheduler.cc event.cc

SRC_C += lxc_emul.c socket.c

CC_WARN  =
CC_OPT  += -fno-builtin-toupper

CC_C_OPT   += -include $(LIB_INC_DIR)/lx_emul.h
CC_C_OPT   += -Wall -Wno-unused-variable -Wno-unused-function -Wno-uninitialized
CC_C_OPT   += -Wno-unused-but-set-variable -Wno-pointer-sign -Wno-cast-qual
CC_CXX_OPT += -Wall -fpermissive

WIFI_CONTRIB_DIR := $(call select_from_ports,dde_linux)/src/lib/wifi
DRIVERS_DIR := $(WIFI_CONTRIB_DIR)/drivers
WIFI_DIR    := $(WIFI_CONTRIB_DIR)/net

#
# The order of include-search directories is important, we need to look into
# 'contrib' before falling back to our custom 'lx_emul.h' header.
#
INC_DIR += $(LIB_INC_DIR)
INC_DIR += $(WIFI_CONTRIB_DIR)/include $(WIFI_CONTRIB_DIR)/include/uapi \
INC_DIR += $(LIB_DIR)

CC_OPT += -U__linux__ -D __KERNEL__ -DCONFIG_PCI
#CC_OPT += -DCONFIG_INET
CC_OPT += -DCONFIG_NET -DCONFIG_NET_NS -DCONFIG_WLAN #-D__MAC80211_DRIVER_TRACE

# needed for firmware loading
CC_OPT += -DCONFIG_FW_LOADER

# bling bling
CC_OPT += -DCONFIG_LEDS_TRIGGERS -DCONFIG_MAC80211_LEDS

# mesh
CC_OPT += -DCONFIG_MAC80211_MESH

# power management
CC_OPT += -DCONFIG_PM -DCONFIG_PM_SLEEP

# rfkill
CC_OPT += -DCONFIG_RFKILL
# choose default pid algorithm
CC_OPT += -DCONFIG_MAC80211_RC_PID -DCONFIG_MAC80211_RC_DEFAULT=\"pid\"

#
# Suffix of global 'module_init' function
#
MOD_SUFFIX =
CC_OPT += -DMOD_SUFFIX=$(MOD_SUFFIX)

SRC_C += lib/ctype.c
SRC_C += lib/crc32.c

# crypto
SRC_C += $(addprefix crypto/, $(notdir $(wildcard $(WIFI_CONTRIB_DIR)/crypto/*.c)))

# net
SRC_C += net/core/datagram.c
SRC_C += net/core/rtnetlink.c
SRC_C += net/core/skbuff.c
SRC_C += net/core/sock.c
SRC_C += net/ethernet/eth.c
SRC_C += net/netlink/af_netlink.c
SRC_C += net/netlink/genetlink.c
SRC_C += net/packet/af_packet.c

# netlink
SRC_C += lib/average.c
SRC_C += lib/nlattr.c
SRC_C += lib/rbtree.c

# wifi mac80211
SRC_C += $(addprefix net/mac80211/, $(notdir $(wildcard $(WIFI_DIR)/mac80211/*.c)))

# wifi rfkill
SRC_C += $(addprefix net/rfkill/, $(notdir $(wildcard $(WIFI_DIR)/rfkill/*.c)))
INC_DIR += $(WIFI_DIR)/rfkill

# wifi cfg80211
SRC_C += $(addprefix net/wireless/, $(notdir $(wildcard $(WIFI_DIR)/wireless/*.c)))
INC_DIR += $(WIFI_DIR)/wireless

# led driver
SRC_C += $(addprefix drivers/leds/, $(notdir $(wildcard $(DRIVERS_DIR)/leds/*.c)))
CC_OPT += -DCONFIG_LEDS_TRIGGERS

CC_OPT += -D__CHECK_ENDIAN__

# iwlwifi driver
DRV_DIR_IWLWIFI := drivers/net/wireless/iwlwifi
SRC_C += $(addprefix $(DRV_DIR_IWLWIFI)/,      $(notdir $(wildcard $(WIFI_CONTRIB_DIR)/$(DRV_DIR_IWLWIFI)/*.c)))
SRC_C += $(addprefix $(DRV_DIR_IWLWIFI)/dvm/,  $(notdir $(wildcard $(WIFI_CONTRIB_DIR)/$(DRV_DIR_IWLWIFI)/dvm/*.c)))
SRC_C += $(addprefix $(DRV_DIR_IWLWIFI)/mvm/,  $(notdir $(wildcard $(WIFI_CONTRIB_DIR)/$(DRV_DIR_IWLWIFI)/mvm/*.c)))
SRC_C += $(addprefix $(DRV_DIR_IWLWIFI)/pcie/, $(notdir $(wildcard $(WIFI_CONTRIB_DIR)/$(DRV_DIR_IWLWIFI)/pcie/*.c)))
INC_DIR += $(WIFI_CONTRIB_DIR)/$(DRV_DIR_IWLWIFI)
CC_OPT += -DCONFIG_IWLMVM -DCONFIG_IWLDVM
CC_OPT += -DCONFIG_IWLWIFI_DEBUG

# iwlegacy driver
DRV_DIR_IWLEGACY := drivers/net/wireless/iwlegacy
SRC_C += $(addprefix $(DRV_DIR_IWLEGACY)/, $(notdir $(wildcard $(WIFI_CONTRIB_DIR)/$(DRV_DIR_IWLEGACY)/*.c)))
INC_DIR += $(WIFI_CONTRIB_DIR)/$(DRV_DIR_IWLEGACY)
CC_OPT += -DCONFIG_IWL3945 -DCONFIG_IWL4965
CC_OPT += -DCONFIG_IWLEGACY_DEBUG

#
# Determine the header files included by the contrib code. For each
# of these header files we create a symlink to 'lx_emul.h'.
#
GEN_INCLUDES := $(shell grep -rh "^\#include .*\/" $(WIFI_CONTRIB_DIR) |\
					sed "s/^\#include [^<\"]*[<\"]\([^>\"]*\)[>\"].*/\1/" |\
					sort | uniq)
#
# Filter out original Linux headers that exist in the contrib directory
#
NO_GEN_INCLUDES := $(shell cd $(WIFI_CONTRIB_DIR)/; find include  -name "*.h" |\
					   sed "s/.\///" | sed "s/.*include\///")
GEN_INCLUDES    := $(filter-out $(NO_GEN_INCLUDES),$(GEN_INCLUDES))

#
# Put Linux headers in 'GEN_INC' dir, since some include use "../../" paths use
# three level include hierarchy
#
GEN_INC         := $(shell pwd)/include/include/include

$(shell mkdir -p $(GEN_INC))


GEN_INCLUDES    := $(addprefix $(GEN_INC)/,$(GEN_INCLUDES))
INC_DIR         += $(GEN_INC)

#
# Make sure to create the header symlinks prior building
#
$(SRC_C:.c=.o) $(SRC_CC:.cc=.o): $(GEN_INCLUDES)

$(GEN_INCLUDES):
	$(VERBOSE)mkdir -p $(dir $@)
	$(VERBOSE)ln -s $(LIB_INC_DIR)/lx_emul.h $@

vpath %.c  $(DRIVERS_DIR)
vpath %.c  $(WIFI_CONTRIB_DIR)/drivers/net/wireless/
vpath %.c  $(WIFI_DIR)
vpath %.c  $(WIFI_CONTRIB_DIR)
vpath %.c  $(LIB_DIR)
vpath %.cc $(LIB_DIR)
vpath %.cc $(LIB_DIR)/signal

# vi: set ft=make :
