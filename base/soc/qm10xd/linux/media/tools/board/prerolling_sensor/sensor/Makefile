

all:
	@for x in `find ./ -maxdepth 2 -mindepth 2 -name "Makefile" `; \
	do cd `dirname $$x`; if [ $$? ]; then make || exit 1; cd ../; fi; done

lib:
	@for x in `find ./ -maxdepth 2 -mindepth 2 -name "Makefile" `; \
	do cd `dirname $$x`; if [ $$? ]; then make lib; cd ../; fi; done

clean:
	rm -rf sensor_common/*.o
	rm -rf sensor_common/*.d
	rm -rf sensor_common/*.a
	@for x in `find ./ -maxdepth 2 -mindepth 2 -name "Makefile" `; \
	do cd `dirname $$x`; if [ $$? ]; then make clean; cd ../; fi; done

install:
	@for x in `find ./ -maxdepth 2 -mindepth 2 -name "Makefile" `; \
	do cd `dirname $$x`; if [ $$? ]; then make install; cd ../; fi; done

prebuild-paraFile:
	@for x in `find ./ -maxdepth 2 -mindepth 2 -name "Makefile" `; \
	do cd `dirname $$x`; if [ $$? ]; then make prebuild-paraFile; cd ../; fi; done

build-paraFile:
	@for x in `find ./ -maxdepth 2 -mindepth 2 -name "Makefile" `; \
	do cd `dirname $$x`; if [ $$? ]; then make build-paraFile; cd ../; fi; done

delete-paraFile:
	@for x in `find ./ -maxdepth 2 -mindepth 2 -name "Makefile" `; \
	do cd `dirname $$x`; if [ $$? ]; then make delete-paraFile; cd ../; fi; done