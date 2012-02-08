SLASHEM_REV = fbd743a
SLASHEM     = slashem-$(SLASHEM_REV)
SLASHEM_DIR = blitz-slashem-$(SLASHEM_REV)
SLASHEM_TGZ = slashem-$(SLASHEM_REV).tgz
SLASHEM_URL = https://github.com/blitz/slashem/tarball/$(SLASHEM_REV)

#
# Interface to top-level prepare Makefile
#
PORTS += $(SLASHEM)

prepare:: $(CONTRIB_DIR)/$(SLASHEM)

#
# Port-specific local rules
#
$(DOWNLOAD_DIR)/$(SLASHEM_TGZ):
	$(VERBOSE)wget -O $@ -c -P $(DOWNLOAD_DIR) $(SLASHEM_URL) && touch $@

# XXX Is there a better way to change the name of the extracted directory?
$(CONTRIB_DIR)/$(SLASHEM_DIR): $(DOWNLOAD_DIR)/$(SLASHEM_TGZ)
	$(VERBOSE)tar xfz $< -C $(CONTRIB_DIR) && touch $@

$(CONTRIB_DIR)/$(SLASHEM): $(CONTRIB_DIR)/$(SLASHEM_DIR)
	$(VERBOSE)rm -fr $@
	$(VERBOSE)mv $< $@

