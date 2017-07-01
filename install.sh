# make is ????

gcc -o /usr/local/bin/osl40562 main.c -l bcm2835



cp ./osl40562.service /usr/lib/systemd/system/osl40562.service
systemctl daemon-reload


systemctl start osl40562
systemctl enable osl40562
