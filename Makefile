ASTYLEFLAGS := --style=attach --indent=force-tab --break-blocks --align-pointer=name --add-brackets --unpad-paren --pad-oper --pad-header

.PHONY: all clean format

format:
	@echo "Astyle code formatter"
	astyle $(ASTYLEFLAGS) -r "*.c"
	astyle $(ASTYLEFLAGS) -r "*.h"
	find -name "*.orig" -delete

