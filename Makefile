# vi: set tabstop=4 noautoindent:

#
all: install


install: 
	[ -d /var/lib/jupyterhub ] || mkdir /var/lib/jupyterhub
	[ -f /usr/local/etc/jupyterhub_lticontainer_config.py ] || install -m 0644 etc/jupyterhub_lticontainer_config.py /usr/local/etc
	[ -f /usr/local/etc/lticontainerspawner_config.py ]     || install -m 0644 etc/lticontainerspawner_config.py     /usr/local/etc
	[ -f /usr/lib/systemd/system/jupyterhub.service ]       || install -m 0644 etc/jupyterhub.service                /usr/lib/systemd/system
	[ -f /usr/lib/systemd/system/ltictr_proxy.service ]     || install -m 0644 etc/ltictr_proxy.service              /usr/lib/systemd/system
	[ -f /usr/local/etc/ltictr_proxy.conf ]                 || install -m 0640 etc/ltictr_proxy.conf                 /usr/local/etc
	install -m 0755 etc/lticontainerspawner.py  /usr/local/etc
	install -m 0644 etc/podman.socket           /usr/lib/systemd/system
	install -m 0755 bin/ltictr_proxy_server     /usr/local/bin
	install -m 0755 bin/ltictr_api_server       /usr/local/bin
	systemctl enable jupyterhub   || true
	systemctl enable ltictr_proxy || true
	systemctl daemon-reload


clean:
	rm -f /var/lib/jupyterhub/*


uninstall: clean
	systemctl stop jupyterhub      || true
	systemctl disable jupyterhub   || true
	systemctl stop ltictr_proxy    || true
	systemctl disable ltictr_proxy || true
	rm -f /var/lib/jupyterhub/*
	rm -f /usr/local/etc/jupyterhub_lticontainer_config.py
	rm -f /usr/local/etc/lticontainerspawner.py
	rm -f /usr/local/etc/lticontainerspawner_config.py
	rm -f /usr/lib/systemd/system/jupyterhub.service
	rm -f /usr/lib/systemd/system/ltictr_proxy.service
	rm -f /usr/local/bin/ltictr_proxy_server
	rm -f /usr/local/bin/ltictr_api_server
	rm -f /usr/local/etc/ltictr_proxy.conf
	systemctl daemon-reload

