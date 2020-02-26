# Game Theory

## 壹,概述
* 專有名詞:
    * N={1,2...n} 玩家人數
        * 單人遊戲 -> dicision theory
        * Zero person game -> 生命遊戲(初始設定後自己運行)(每回合細胞死亡或重生取決於周圍細胞數)
    * 擴展形式賽局(extensive form game)
        * 玩家可以知道自己的處境(position)
        * ==combinatory game==(組合對局理論):所有玩家都有資訊完整(perfect information),無機率
        * impatial(公平):每個玩家的選擇集合相同
    * ==策略賽局==(strategic form game) 
        * 部分選擇與獎勵不清楚
        * 每個玩家選擇自己的策略集合A~i~={ a~i1~, a~i2~ ... }(必須考慮所有其他玩家可能的策略)
        * ==獎勵函數==(payoff function)= f~j~ (a~1~, a~2~,... a~n~ ) 其中 a~n~是玩家n選擇的策略
   * 預期效用(utility theory)
       * 內容: 在有風險的情況下,理性人如何決定以獲得最大期望值
       * 用途: 利用預期效理論生成數值(numerical)獎勵
   * ==零和賽局==(zero sum game)
       * 定義:所有 f~j~ 之和為零
       * finite game: 策略集合有限大
       * ==minimax theory==: 所有玩家==必有最優策略==,保證自己在不知道對方選擇時獲利最高 
           * 任意行的最高點一定比任意列的最低點高
    * 非零和賽局(non-zero game)
        * 不存在最佳解(optimal choose)
        * noncooperative theory: Nash均衡
        * ==cooperative theory==: 玩家可已形成強迫行協議(binding agreement),
        * transferable,side payment: 獲得獎勵可以在聯盟中分攤,且所有成員衡量標準同
    * 多人賽局(合作為主)
        * 研究合作的獲利與動機
        * Grand Coalition: 所有人共同合作
        * core of economy: 一個利益分配模式(所有人都無法以單方面行為優化)
        * Shapley value: 當一群玩家合作獲得共同獎勵後應當如何分贓 **(?**
        * nucleolus: **(?**

## 貳,Impartial Combinatorial Game
* impartial: 雙方玩家每回合能做的行動相同 <-> partizan(如棋類)
* combinatorial game
    * 玩家數=2
    * 可形成有限的position
    * 在有限步內結束遊戲(若無,必加結束條件)
    * 玩家輪流行動(不能同時行動)
    * 資訊完全(不涉及機率,如發牌)(所有行動公開)
### Take Away Game
* 規則: 開始場上21棋子,每回合可拿1-3顆,最後拿的獲勝
    * ==P-position==(previous): 當形成如此場面時,上一個行動的玩家會獲勝(如:4,8,12,16等)
    * ==N-position==(next): 當形成如此場面時,即將行動的玩家會獲勝(如:1,2,3,5,6,7,9等)
* ==算法==: 
    * 把terminal position定為P-pos
    * 把一步後會變成ter pos定為N-pos
    * 把經一次移動後必成為N-pos的場面定為P-pos
    * 重複step2,3
* ==**性質**==:
    * terminal pos必為P-pos (此規則下)
    * ==N-pos必能用一步走到P-pos==
    * ==P-pos下一步必為N-pos==
    * 不能同時為P-pos,N-pos
    
* 舉例1: subtraction game:
    * 規則:每回合可以拿{1,3,4},最後拿勝
    * 算法:
        * 1: P-pos = 0
        * 2: N-pos = 1,3,4 
        * 3: P-pos = 2 (因為2只能到1)
        * 4: N-pos = 5,6 (因為 2+3&4 )
        * 5: P-pos = 7 (因為7只能到6,4,3)
        * 重複 3-5
    * 起始=100 -> 後手玩家獲勝
* 舉例2: thirty-one game 
    * 規則: 場面上有1-6張各四張,玩家輪流翻面,先讓總點數破31者輸遊戲
    * 算法:
        * P-pos = 30,23...,9,2
        * N-pos = 29,28,27,26,25,24...
        * 拿法1 = 2,4,3,4,3,4,3,4,3,1 (先手勝)
        * 拿法2 = 2,5,2,5,2,5,2,5,1,1,1 (後手勝)
        * 法二利用每種牌只有4張的特性獲勝
* 舉例3: empty-and-division
    * 規則: 兩個箱子有(m,n)個棋子,每次移動清空其一箱子並將另一個箱子內的棋子數平分到兩個箱子,箱子不為空,最後拿獲勝
    * 算法:
        * Terminal-pos = (1,1)
        * N-pos = (2^2n+1^,x) & (1,x)
        * P-pos = (2^2n^,2^2n^)
* 舉例4: Chome---Game
    * 規則: 在mxn的棋盤上,每回合玩家可以拿走給定點右上角的所有餅乾,拿到最左下角餅乾的玩家敗
    * 欲證: 先手玩家必有致勝策略
        * Zermelo定理: 步驟有限&信息完備&沒有平局 -> 至少有一方有必勝策略
        * 證明: 設後手有致勝策略,先手拿最右上角,後手任意拿,先手就知道必勝策略
* 舉例5: dynamic-game
    * 規則: 拿到最後者獲勝,每回合不能拿多於上手玩家的量
    * 算法: 
        * 把n分解為2進制: 44=101100~2~
        * 每次拿最小的2的次方,對方拿完後繼續使用此策略 -> 先手玩家勝 
        * n=2的次方數 -> 後手玩家勝
* 舉例6 Dynamic substraction game:
    * 規則: 拿到最後者獲勝,每回合最多拿上回合拿的兩倍
    * 算法: 
        * 拿最小的費氏數 -> 先手勝
            * Zeckendorf's THM: 所有整數可以唯一表示為非鄰近的費氏數之和 
            * *pf:數歸,1~4成立, F~j~ < n < F~j+1~ ,a = n - F~j~ ,a != F~j-1~ -> 即證*
            * 思路: 
                * 費氏數下兩位比前兩位的2倍大 -> 對方無法模仿策略
                * 對手拿完後此策略可延續
                * 4以上採用此戰術
        * n = 費氏數 -> 後手勝
* 舉例7 SOS game: 
    * 規則: n個連續空格,玩家輪流填入S/O,先完成SOS者勝
    * 算法:
        * S__S -> P-pos(X-rates)
        * N=奇數時 -> 先手勝 ; N=偶數 -> 後手勝　　
        * N=14 : O放在7平手

### Game of Nim
* 規則: 有三疊棋子,每次可以拿任一疊中的任意數量棋,拿到最後者勝
* ==Nim-Sum==(以下用+表示): 兩數NimSum = 先換成2進制再相加,不進位 -> 滿足交換&結合&消去率(即x=y,y=z->x=z)
* ==Thm== : Game of Nim 中 P-pos 滿足所有疊數的Nim-Sum 為零
* 性質(驗證):
    * Terminal point = P-pos <- (0,0,0) = 0
    * N-pos必能以一步到P-pos -> 更改最大疊的數值至和為零
    * P-pos下一步必為N-pos -> X~1~' < X~1~ ; 若X~1~+X~2~+X~3~ = X~1~'+X~2~+X~3~ -> X~1~=X~1~' (矛盾) 即下一步 Nim-Sum 不為零,為 N-pos
* Misere Nim: 前面照常,當對手拿到剩下一疊時大於1時,把它拿成1/0 -> 先手玩家勝

* 舉例1 Nimble Game: 
    * 規則: 有一行連續空格,上放有硬幣,每格可放不只一個,每回合玩家移動一枚硬幣至他的左邊,當所有硬幣在0th格時遊戲結束,最後移動的玩家獲勝.
    * 算法: NimGame一種,每個硬幣代表一疊,所在格數代表棋子數
* 舉例2 Turning Turtle:
    * 規則: 一行硬幣正反不一,每回合將一人頭翻成反面,並可以選擇將其左邊任一硬幣翻面
    * 算法: NimGame一種,在第n個位置的人頭,視為有n個的一疊 ( 當將n,k兩個人頭翻面時,Nim的意義是同時移除兩整疊,但其NimSum只改變n-k,因為k+k=0 )
* 舉例3 Northcott's Game:
    * 規則: 每行有一黑一白棋,每回合可以在行內任意移動,但不能跳躍對方
    * 算法: NimGame一種,黑白棋間距視為每一疊的數量,若對方後退,可以往前以抵銷(reversible) 
* 舉例4 StairCase Nim:
    * 規則: 每回合可移動某一階梯上任意數量至下一階,到底層的被移除,最後移動者獲勝
    * 算法: 
        * 把奇數階上的棋子視為一堆N個的棋子,移到偶數堆視為移除,偶數對移到奇數堆視為增加(Nim變形),計算NimSum可得必勝策略
        * 對手移動棋子 -> 再將相同數量的棋子往下移一格,回到原本NimSum
        * 偶數階不行: 因為最後會剩下第2階有棋子,此為N-pos
    * 結論: 當奇數階NimSum=0時為P-pos
* 舉例5: ==Moore's theory==
    * 規則: 每次可以移動k堆中的任一數量棋子
    * ==結論==: P-pos為所有堆換為2進制,再以k+1進制做NimSum
    * 性質(驗證)
        * Terminal-Point = P-pos <- (0,0..)為P-pos
        * P-pos下一步必為N-pos : 只能動k步, 但要mode(k+1) -> 下一步必為N-pos
        * N-pos必能用一步走到P-pos : (k+1)-(至少為1) < k -> 必可形成P-pos

### Graph Game
* Directed Game :　
    * 定義：G(X,F)
        * Ｘ位置集合
        * Ｆ(x):每個ｘ屬於Ｘ有對應的Ｆ(x),代表可以從ｘ點移動到的點
        * TerminalPoint := F(x)=空的點
        * Path := 一個符合移動規則的數列　
        * ProgressivelyBounded := Path長度有限且沒有迴圈
    * 規則 : 從點X~0~開始移動,每回合選擇F(x)中的一個點,走到TerminalPoint的下一家敗
* ==Sprague-Grundy Function==
    * G(x)函數值　＝　不在F(x)中的最小整數．其中TerminalPoint的S.G.value = 0
    * ==定理==： P-pos 為 G(x)==0的點
    * 驗證：
        * TerminalPoint = P-pos
        * P-pos下一步是N-pos: G(x)==0 -> 下一步G(x)必不為零
        * N-pos必能以一步到P-pos: 易知
* Ｘ無窮：
* Ｘ迴圈：
* 舉例１　SubtractionGame（可移除１～３）：
剩餘棋子數　０　１　２　３　４　５　６　７　．．．
S.G.value　０　１　２　３　０　１　２　３．．．
* 舉例2 Wythoff Game
    * 規則: 每次只能往左\往下\往左下移,最後移動者勝
    * S.G value ![](https://i.imgur.com/kTMpliT.jpg)
    * 等价于: 2堆棋,可以拿取一堆中的任意数量/两堆中相同的数量
    * 3维时: NimGame壹种,因为拿三堆中相同数量的棋必改变NimSum
* ==定理==: SubtractionGame中S若为有限集 -> S.G.value必有周期性
* 举例3: 图片题:
    * ==解法==: (2th)该点到的两个点都指向0 -> 此点=0 (4th)指向的量个点为0,>=2 -> 此点=1
    * ![](https://i.imgur.com/qjDtCKA.jpg)

### Sum of Combinatorial Game

* 定义: 同时玩多个Game
* 定理1: SumOfComatorialGame的S.G.value是分别游戏的NimSum
* 定理2: 所有impartialGame可以被简化为NimPile,数量是S.G.Value

* 举例1: TakeAndBreakGame:
    * 规则: 移除一Pile中任意数量chip/将任一Pile==拆分==
    * 算法:
        * ==先算单一pile的S.G.Value==: 0,1,2,4
        * (3->[(1,2),2,1,0] 因此SGValue=4) 
        * 如: (2,5,7) -> (2,5,1,6)
* 举例2: Kayles Game
    * 规则: 一排Pin每次拿走1个或邻近的两个(等价于每次拿一或两个,并且能够选择是否把pile拆分)
    * 算法: 列出S.G.Value发现12一循环(但有些例外)
* 举例3: GraphGame
    * 规则1: 图为一折线,每次拿走一个点与其邻边(等价于每次可以拿1chip/2chip并拆分pile
    * 算法: 

    | chip     | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 
    | -------- |---|---|---|---|---|---|---|
    | SG value | 0 | 1 | 2 | 0 | 1 | 2 | 3 |
    * 规则2: 图为一n边形,其他同上
    * 算法: 第一次拿走2chip后回到上表
* 举例4: Grundy Game
    * 规则: 起始pile*1, 每次必须拆分为2个不等大pile,最后移动获胜
    * S.G.value: 0 0 1 0 2 

### Coin-Turning Game

* 概述:
    * 规则: 一排硬币正反不一,每次翻转任意量硬币,但最右边一定要是头->反(progressively bounded)
    * SumOfCombinatorialGame一种,每个h视为一个Game
    * 如: THHTH 可视为 TH + TTH + TTTTH
#### 一般
* 举例1: Subtraction Game
    * 编号: 0(假想),1~x
    * 规则: 每次得拿第x个与第x-y个,y属于S{}
    * 如: x=5,S={1,2,3},(T)TTTTH -> (T)TTTH (等价于拿一个) 
* 举例2: MockTurtle 
    * 规则: 每次可翻1~3个,必须使最右边的head -> tail
    * 算法:(为何从0开始???)
        * 先算1个Game的S.G.value 

            *   | chip     | 0 | 1 | 2 | 3 | 4 | 5  | 
                | -------- |---|---|---|---|---|----|
                | SG value | 1 | 2 | 4 | 7 | 8 | 11 |
        * S.G.value是2进制的odious数
            * odious: 2进制中,有奇数个1, Even: 2进制中,有偶数个1
        * P-pos: NimSum = 0 且 有偶数个pile
            * 因为: odious (Nim+) odious = even
* 举例3: Ruler
    * 规则: 每次可翻连续的任意个,必须使最右边的head -> tail
    * 特色: x的S.G.value = 2^n^, 其中2^n^是可被x整除的最小数(找规律)
* 举例4:
    * 规则: 编号从0开始,0假想,每次翻chip*4包含最右边H->T,最左边,n,X-n
    * 等价于grundy Game
  
#### 二维 
* Nim Multiplication
    * 2的2^n^次方: 2,4,16,256...
        * x (Nim*) x = 1.5x
        * x (Nim*) y = x (real*) y (其中X > y)
        * 0 (Nim*) 任意数 = 0
    * 非上述数
        * 拆分为x的NimSum
* Tartan THM:
    * Game1 X Game2 的 S.G.Function = G~1~(x) (Nim*) G~2~(y)
* 举例1: Acrostic Twin
    * 规则: 每次翻2chip(必须同行/列), 必含右下角H -> T
    * S.G,value![](https://i.imgur.com/q7q3dqs.jpg)
        * 行列编号从0开始 (红)
        * S.G.value 是行列编号NimSum (黑)
* 举例2:
    * 规则: 每次翻4chip分别是矩形的4顶点,且右下必为H->T
    * S.G.value
        * 0 0 0 0
        * 0 1 2 3
        * 0 2 2 1
        * 0 3 1 2
    * 发现: G(x,y) = x (nim*) y
        * Tartan THM: 此游戏为 TwinGame*2 且 TwinGame G(x)=x 
* 举例3: Rugs Game 
    * 规则: 每次可翻一个矩形的所有硬币,最右下H->T
    * 等价于RulerGame^2^
* 举例4: Tartan Game (没懂)
    * 定义: 所有Game^2^ (不等于SumOfCombinatorial  Game)
    * 解法: 将第一行/列的S.G.value填上,做Nim*
    * 改变S.G.value理论可行,但实际上要该翻哪些按以下方法:
    * ![](https://i.imgur.com/g16uERP.jpg)
(跳过此段习题)
            
### Green Hackenbush

* 规则: 移除任意边,当图形于地面失去连结将被移除
* 举例1: banboo
    * 所有图形为一根根直线,每根视为一个pile
* 举例2:
    * 所有impartialGame可以被简化为NimPile,整体游戏SGvalue是所有pile的NimSum
    * Colon Principle: 每个顶点SGvalue = 上面所有分支的NimSum
        * 证明: 当G~1~/G~2~SGvalue相等,假设有两个相同图形的相同位置上连接G~1~&G~2~欲证两图形NimSum = 0
        * 过程: 即此点为P-pos,当P玩家使用战术为对称拿法时必胜(即证)
        * 寻找切割点: 简化时由上往下,寻找时倒叙
    * Fusion Principle: 地面所有点视为同一点,回圈所有点视为一点 + n个长度为1的pile(n回圈内点量)

## 叁, Two-Person Zero-Sum Game


---

###### tags: `Economics`
