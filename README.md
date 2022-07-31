1.小组成员：	
刘庭杰  账户名称：scattered-shadow

荆泽宇	账户名称：Jzy604948

2.项目介绍
已完成项目：1、2、3、7、17

Project1: implement the naïve birthday attack of reduced SM3

对SM3的简单生日攻击

完成人：刘庭杰 荆泽宇

对应代码：SM3.cpp SM3_birthday_attack.cpp
代码说明：
由荆泽宇完成SM3的代码实现，刘庭杰完成生日攻击部分

在生日攻击时我们对前32位进行了生日攻击，通过随机选取\sqrt{n}个数据，判断其中是否有相同结果，从而得出是否找到一组碰撞。经过100次测试后发现成功率在60\%
符合算法成功概率


运行截图：
![birthday_attack1](https://user-images.githubusercontent.com/57316569/182013943-1bdacc3d-e488-461c-aa60-d9a0b5bfc543.PNG)

![SM3_birthday_attack2](https://user-images.githubusercontent.com/57316569/182013945-7d642f1c-a984-42f5-b794-13f63f468974.png)


Project2: implement the Rho method of reduced SM3

完成人：荆泽宇

对应代码：Pollard Rho.cpp

代码说明：
根据生日悖论原理，我们选出 x1, x2, x3, x4, x5 …… xn 个数，其中两个的差是 n 的因数或者与 n 的最大公因数大于1。
对于第一种，我们要考虑的是固定的 p 和 q 整除 N，只有两个。
对于第二种，我们要考虑的是组成的数和 N 的最大公约数是否大于 1。
显然第二种情况更能找出因数。

因此我们的方法是在 [2, n-1] 中选取 k 个数，如果 gcd(xi - xj, N) > 1 ，那么我们就找到了相应的因数。


运行截图![屏幕截图 2022-07-31 113842](https://user-images.githubusercontent.com/57316569/182013824-9e4bf67c-cf3c-45ae-b2e0-5a066a9b7ba7.png)


Project3: implement length extension attack for SM3, SHA256, etc.

完成人：刘庭杰

对应代码：SM3_birthday_attack.cpp

代码说明：
长度扩展攻击基本思想 

a=xxxx

b=sm3(a+填充)
c=sm3(b+a)

运行截图![len_attack](https://user-images.githubusercontent.com/57316569/182013691-7cf1f16f-b254-47f6-b28b-14fe2edc14fe.PNG)

*Project7: report on the application of this deduce technique in Ethereum with ECDSA

完成人：荆泽宇

对应代码：ECDSA报告.pdf

代码说明：

对ECDSA的来源，构建，以及应用进行了学习和研究

Project17: research report on MPT

完成人：刘庭杰

对应代码：research report on MPT.pdf

代码说明：

对MPT的来源，构建，以及应用进行了学习和研究

3.贡献说明

刘庭杰：sm3生日攻击、MPT报告、长度扩展攻击

荆泽宇：sm3算法实现 sm3的rho算法实现、ECDSA报告
