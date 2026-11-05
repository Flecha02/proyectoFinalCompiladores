<<<<<<< HEAD
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
=======
push_dir b
call input
t1=1+2
t2=t1+b
a=t2
push a
>>>>>>> 0d13e2d360bb01cbac9ff8a5e01d73312fac9992
call output
