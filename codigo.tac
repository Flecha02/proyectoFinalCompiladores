push x
call input
pop x
push y
call input
pop y
t1=x<10
t2=y>0
t3=t1 and t2
ifZ t3 goto l1
res=1
goto l2
l1:
res=0
l2:
push res
call output
