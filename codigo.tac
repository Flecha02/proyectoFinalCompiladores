push_dir x
call input
push_dir y
call input
t1=1+x
z=t1
t2=x<y
ifZ t2 goto l1
t3=x+y
z=t3
goto l2
l1:
z=0
l2:
push z
call output
