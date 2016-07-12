<?php

//exec('mysqldump --user=355840_dsgenie2 --password=WCuq32w7 --host=mysql51-039.wc1.ord1.stabletransit.com 355840_dsgenie2 | bzip2 >  /mnt/stor17-wc1-ord1/355840/www.dsgenie.com/db2016-1.sql.bizp2');

exec('bunzip2 < /mnt/stor17-wc1-ord1/355840/www.dsgenie.com/db2016-1.sql.bzip2 | mysql --user=355840_dsgenie3 --password=WCuq32w7 --host=mariadb-149.wc1.ord1.stabletransit.com --database=355840_dsgenie3');



