#!/bin/sh
#
# 用来监视系统中的用户数(加入、退出情况)
# watch.sh -a simple version of the watch utility

	who | sort > prev  #get initial user list
	while true	   
	do
		sleep 30
		who | sort >current   #get curent user list 
		echo "Logged out:" 
		# comm 比较会得到三个集合: 1代表prev独有的行，2代表current独有的行，3代表共有的
		comm -23 prev current  # 通过比较，得到退出登录的,只显示prev独有的行
		echo "Logged in:"
		comm -13 prev current  # 通过比较，得到新加入的
		mv current prev  　    # prec = current
	done
