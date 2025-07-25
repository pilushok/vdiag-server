SUBDIRS := uds_server uds_handlers  

.PHONY: all clean install $(SUBDIRS)

all:
	@echo " MAKE	build" 
	@set -e; for dir in ${SUBDIRS}; do make -C $$dir all; done

clean:
	@echo " MAKE clean" 
	@set -e; for dir in ${SUBDIRS}; do make -C $$dir clean; done

install:
	@echo " MAKE	install" 
	@set -e; for dir in ${SUBDIRS}; do make -C $$dir install; done

test:
	@echo " MAKE	test"
