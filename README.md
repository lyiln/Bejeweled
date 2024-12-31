这是一份完全基于qt6开发的大二的宝石迷阵实训项目。
本项目是实现了简单的登录（使用数据库）界面，菜单界面，游戏界面。
登录注册界面
![image](https://github.com/user-attachments/assets/133f989c-8ed8-48e6-8105-b5c162ce7cb9)
玩家输入用户名与密码，如果为未注册用户，点击注册即注册成功，再点击登录，即可进入主界面；如果为已注册用户，若密码正确，点击登录即可进入主界面，若密码错误，则提示密码错误重新输入，直至密码正确。
菜单界面
![image](https://github.com/user-attachments/assets/2cfe915c-11f0-415a-9494-bb8eb4156308)
玩家可选择不同的模式进行游玩：
	（1）普通模式：可供玩家进行无尽玩法，但考虑现实，实际限时9999秒结束。
	（2）限时模式一、二：限时较短，分别为60，30秒，玩家需要在规定时间内达成尽可能多的分数。若未在规定时间内达到三十分，则会显示you lose，反之则为you succeeds
	点击退出，结束程序。
游戏界面
![image](https://github.com/user-attachments/assets/5632b2da-5c86-4815-85fb-a9d837d02c85)
（1）玩家通过点击两个相邻的宝石进行交换，如果交换后判断横竖方向有3个及以上相同宝石即可消除，否则宝石回退到原来位置。
	（2）消除宝石后上方宝石下移，且最上方补充新的随机宝石，分数根据消除宝石的数量来计算。
	（3）玩家点击鼠标右键再按下“B”键，即可触发“炸弹”道具，将3×3范围内的宝石全部变成右键点击的宝石，完成大范围的消除。
	（4）当玩家自己找不到可以交换匹配的宝石时，可以按下“H”键进行提示，寻找可以交换匹配的宝石自动交换，若游戏界面没有可交换匹配的宝石时，按下三遍“H”键即可刷新整个界面。
	注意：炸弹与提示消除的宝石均会有相应的扣一定分计算。
	（5）倒计时结束，即游戏结束，显示最终得分。
 得分显示
 ![image](https://github.com/user-attachments/assets/6faa6f98-d676-4395-a065-84241694d517)
	玩家每次结束游戏后，会有最终得分显示，记录此次游戏的得分，且包含历史最高分数（此次游戏分数不计入历史最高分）。
	点击“Exit”或直接关掉Game Over窗口即可退出返回菜单，若要结束游戏，可在菜单界面点击退出。
