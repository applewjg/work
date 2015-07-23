SUBDIRS   = make
$(OBJS): subdirs

subdirs:
	@if [ -d output ]; then rm -rf output; fi
	mkdir output;
	echo "VERSION=$(VERSION)" > version;
	@for i in $(SUBDIRS); do \
		echo "===> $$i"; \
		cd $$i && $(MAKE) || exit 1; \
		cd ..\
		echo "<=== $$i"; \
		done

clean:
	rm -rf output
	@for i in $(SUBDIRS); do \
		echo "===> $$i"; \
		cd $$i && $(MAKE) clean || exit 1; \
		cd ..\
		echo "<=== $$i"; \
		done
