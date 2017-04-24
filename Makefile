
default:	build

clean:
	rm -rf Makefile objs

build:
	$(MAKE) -f objs/Makefile

install:
	$(MAKE) -f objs/Makefile install

modules:
	$(MAKE) -f objs/Makefile modules

upgrade:
	/home/centos7/nginx/sbin/nginx -t

	kill -USR2 `cat /home/centos7/nginx/logs/nginx.pid`
	sleep 1
	test -f /home/centos7/nginx/logs/nginx.pid.oldbin

	kill -QUIT `cat /home/centos7/nginx/logs/nginx.pid.oldbin`
