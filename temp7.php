<?php


//phpinfo(); exit(0);
//exec('mysqldump --user=355840_dsgenie2 --password=WCuq32w7 --host=mysql51-039.wc1.ord1.stabletransit.com 355840_dsgenie2 | bzip2 >  /mnt/stor17-wc1-ord1/355840/www.dsgenie.com/db2016-1.sql.bizp2');

//exec('bunzip2 < /mnt/stor17-wc1-ord1/355840/www.dsgenie.com/db2016-1.sql.bizp2 | mysql --user=355840_dsgenie3 --password=WCuq32w7 --host=mariadb-149.wc1.ord1.stabletransit.com --database=355840_dsgenie3');
//print 'start';

//exec('mariadump --user=355840_dsgenie3 --password=WCuq32w7 --host=mariadb-149.wc1.ord1.stabletransit.com 355840_dsgenie3 watchdog | bzip2 >  /mnt/stor17-wc1-ord1/355840/www.dsgenie.com/db2016-2watchdog.sql.bizp2', $out);

//exec('mysqldump --user=355840_dsgenie3 --password=WCuq32w7 --host=mariadb-149.wc1.ord1.stabletransit.com --database=355840_dsgenie3 355840_dsgenie3 | bzip2 >  /mnt/stor17-wc1-ord1/355840/www.dsgenie.com/db2016-2watchdog.sql.bizp2');

//print_r($out); 




exec('tar -czpvf /mnt/stor17-wc1-ord1/355840/www.dsgenie.com/sitebackup.tar.gz /mnt/stor17-wc1-ord1/355840/www.dsgenie.com/web/content/');
