# 台大計算機概論

---
## 資料儲存
為何二進位binary?
符合邏輯，五伏特零伏特→容錯率高unambiguous

XOR 兩者為真兩者為假都為0
OR兩者為假為0

FlI p-flop正反器，可以存一個零一，有state static memory，四個結構:兩個input，set0,set1,if no input preserve 0/1，11 undefine，需要供電才能用，久了會忘，

16進位，0~9A~F，表要很熟

Memory cell→8bits=1bite，最左邊most significant最右邊least significant

Random accessible (相反於循序存取)可按任意順序存取,刪除與插入會比較方便

Dynamic memory(DRAM)久了會忘因為電容過小約奈米非絕緣，refresh circuit，優點小，缺點:隨時讀取後刷新。
DDR一個時脈可以存兩次，電壓降低，耗電少，充電快。
Dual channel兩個記憶體並行

K十的三M6G9T12

大量儲存裝置:硬碟more capacity, less volatility(揮發),slower,online or offline.

Flash drive 不易忘例如COM。雷射改變形狀，磁場。

硬碟Access time=seek tine探針移動時間+rotation delay轉過來的時間

Transfer rate 電路delay，SATA6

Physic/Logical record,檔案通常不連續存因為會刪除等等,硬碟

Buffer緩衝區，把分散資料整理到傳給電腦，等一段時間在把這段時間中蒐集的所有資料一次送到CPU。

Ascii→7，UNICODE 16，ISO STANDARD→32，A=65,

10進位變2進位:一直除二得到餘數得最後一位數，想十進位

影像表達法
Bit map technic: pixel
Vector向量:放大不容易失真，不會鋸齒，可以記弧線

聲音表達法
Sampling:每過一段時間記一次，sample rate 隔多久記一次，beat resolution，y軸精確度。兩個相乘為每秒要記多少bits，BPS bits per second。
MIDI合成:只能記某種聲音，撥預錄檔

數字表示法binary system
減法，定義負數

2補數法two's complement
3bit表示-4~3，9=1(mold8)
令7=-1,6=-2,5=-3,4=-4，好處正的不變且加法成立且判斷正負容易，2-1=2+7=9=1

-2算法:(1)8進位，8-2=6，所以表示六，(2)右邊開始照抄到第一個一，第一個一後開始抄相反的。

EXCESS　
`01234567`→-4-3-2-1,0123，好處:比大小判斷正負容易，excess+"4"(3bits的時候)=binary。

Overflow，算數出來的值無法表示。在two complement中，兩個正的相加可能變負的，兩個負的相加等於正的，因為5=-3(mold8)，一正一負不會。

小數
Fixed point，0.1=1/2 ;0.01= 1/4。
Floating point 浮點數，第一位0正1負sign，二到四指數位exponent(excess)，五到八假數mantissa，假數小於一大於零/ex:0001=16/1

Truncation error:浮點數記法記憶不夠，導致所記的值與實際值的差異。

Normalized form:強制規定exponent第一個數比為一，在IEEE中exponent 0101= 1.0101，但0例外.0 000 0000 =0 !=1/16

(-1)^s*1.mmmm x 2^(eee-4)

4+1/4=4(mold8)
1/4+1/4=1/2
4+1/2=9/2
數值方法:安排誰先算

0.1換成二進位換不完，所以要int float double，因為都是趨近值。

Data compression資料壓縮:
Lossless非失真壓縮。run-lenghth,100個零，frequency-dependent如Huffman code，dictionary coding給常出現的某個東西命名。
LOSSY失真壓縮。影像壓縮relative/difference找相似點，temporal masking拔打的時候記他聲音就好。

傳輸方法:
Huffman 畫樹狀圖，保證只有一種解，但要傳tree去
LZW方法:字典壓縮法，但不用傳字典，例如:xyx_ xyx_ xyx →1213434。看到空白把前面當成一個字存進字典。

避免傳送錯誤，加redumdancy。
error detection知道有錯但沒辦法修，可以請對方重丟,error code，Parity bits 每八個bit加一個bit檢測正確與否（讓八個加起來是奇數）。RAID，磁碟陣列，備份，大小只要1.5倍，第三個用前兩個or（一個不見可以用另外兩個推出）。
Error correcting，hamming distance 為有幾個零一不一樣的，hamming code 找到跟原本的值差多少hamming distance 最少的距離及為值。Hamming code(7,4):畫排容原理圖，可以知道哪個出錯（要傳的值放在中間四個，其他讓大圈圈補到奇數）。

## 計算機結構
central processing unit=CPU
Register 暫存器
Bus與記憶體溝通橋樑
CPU只能對暫存器工作不能對周邊裝置

Load 主記憶體到CPU
Store CPU 到主記憶體
I/O CPU 對其他周邊裝置

記憶體種類
RISK指令簡單，規定大小
SISK指令複雜

暫存器種類
Pc.存運算到的位置
IR  把Pc位子裡的資料丟到這個裡面

組合式語言store,5 , A,7。把暫存器五的資料存到記憶體A7中，機器語言15A7
C語言為高階語言
Compile 將高階語言轉成機器語言
Link 將library使用在compile中

Machine cycle 
Fetch,decode,execute（pc++)
Clock 時脈，一秒幾次cycle

ALC算數運算單元
Logic Shift 1100 左移後1000補零
Arithmetic shift 第一位不變， 後面的其他數logic shift，c++用的，例如：11010000右移變11101000保留第一位因為是正負號。
Rotatation 變一個圈。
Int a=1<<7;a=10000000

Bus有屏寬限制，電壓代表01,因此一次只能少部分人丟訊息給cpu(monitor,disk,cd driver等）因此有controller會等控制等到一定數量後一起傳。SATA 硬碟的controller 協定。

Port 埠，為了簡化指令集，給一個簡單的記憶體位置紀錄某外接裝置

協定
DMA.direct memory access,外部裝置要存取資料在主記憶體，只要通知cpu一次就可以無限存了
Hands shaking 確認開始結束傳送

Pipe line（pre-fetch)技術，cycle的三個步驟平行進行，因為三個步驟不同地方執行。困難：go to，前面白抓了。用預測來優化，如for迴圈。

Parallel 平行處理，MISD, MIMD, SIMD, SISD，MISD:多個部分同時平行運行M，但只有單一來源S。SIMD一個指令加在多個資料上，向量化。因為電腦一次計算一定要64bit，若你要算的東西只要32bit可以用類似向量法一次算兩個。
Parallel vs distribute,平行會共用記憶體，分散會藉由網路連結。seti,NASA 發明的分散式，幫他計算，螢幕保護程式。
平行困難：下一行取決於上一行的值時不行。A[i]=A[i-1]*2可以變成變成奇數項與偶數項拿去*4這樣變兩倍快。
synchronization,工作分配可能不平均，算完的要等沒算完的。
reliability,分散式運算，不會因某個電腦斷線而影響整體。

照片（多核心提升的效能）scaling 
Cp需要跟其他裝置溝通
P最多能平行運算的比例
M全部的工作
## 作業系統
作業系統用處：最大限度的利用你的硬體。

FIFO:first in first out。

Batch 批次處理：累積到一定量一次處理 >interactive隨時與電腦即時互動。
Real time，response time 回應的時間重要，反應時間要短如軍事與醫療。
Time-sharing&multitasking 讓使用者一次使用多個程式雖然只有一個核心。每隔一段小時間given time輪流做某件事。感覺能同時做許多事。
Multiprocessor多核: scaling

Software 分成 application&OS
OS分成shell(使用者能用到) &kernel
Shell:如：GUI圖形使用者介面，window manager如視窗
Kernel:
file manager檔案管理(檔案系統）
device driver 裝置驅動程式（認得外階裝置）
memory manager記憶體管理（主記憶體管理，虛擬記憶體管理～把暫時用不到的資料存到硬碟,但速度很慢）（paging技術把記憶體的資料分成一頁一頁的，把不用的頁丟到硬體）。
kernel中的kernel：
scheduler 排成：維持process table、移除記憶體值、排定優先順序，
dispatcher派遣：context switch把不要的存回去要的存進來(切太短會浪費時間）、執行interrupt後使cpu去執行OS

SSD 固態硬碟，讀寫有次數限制。compile就是反覆讀寫。
Virtual machine: virtual box記憶體要夠大
GNU Linux特化出Android (freeware, open source).VND特化出apple.

Booting (boot strapping不求人）：
boot loader 開機由他呼叫OS把他從硬碟拉到主記憶體read only: EEPROM :erasable 避免這個壞掉就正台電腦壞掉。
BIOS 調整booting sequence 或超頻。

Process 正在運作的程式。
Process state: 被關掉把狀態存起來直到下一次被呼叫。如：虛擬記憶體位置、變數的值。
Process table :process的內容或優先順序管理，如工作管理員。

Mutual exclusion 絕對不能同時處理的事，如共用變數。
Critical region 在執行某程式時沒有其他人能access同個程式，如寫入某程式
Semaphore: atomic test-and-set判斷跟執行間不能中斷。看到沒旗(沒人使用)就插旗，結束拔旗，看到有旗則等待。

Deadlock死結必要條件：
（必要條件：要發生一定要有的條件，有條件不一定會發生）
1.競爭不可分享的資源
2.partial basis 執行的過程中會要求新的資源
3.無法強制拿回記憶體
避免方法：ㄧ，強制搶回記憶體，但可能出錯。二，不能先拿部份資源，可能永遠拿不到資源starvation, aging可以解決 等越久優先序越高。

Security,
Bad habit 密碼強度不高
Auditing software 偵測惡意行為
Sniffing software 鍵盤記錄器
Virus 把自己的負載在其他黨上，不執行該受感染黨就沒事
Worm 本身可執行
Trojan horses 木馬程式，偽裝成善意程式但執行其他功能
OS能夠設定使用者權限，讓非root的程式不能接觸到所有記憶體

## 網絡
LAN:Local area network區域網路（通常指前三個位子一樣）：虛擬IP:193.168.@.@ or 10.@.@.@外面不能直接找到你
MAN:metropolitan area network有一定規模的區域網路
WAN：Wide area network

Bus: 所有訊號廣播到bus上
Ring:只能知道下一家是誰
Star:所有電腦都拿到中心
Hub:通電的話bus能增強訊號

Protocols 協定
Ring上的：token ring: 拿到token的人才能發言，把token往下一家傳。token如果消失，隨即指定一個人當下一個token。
CSMA/CD:wire如果兩人同時要講話，一起忍一個隨機的時間，直到只有一個人要講話為止，再碰上就再一次。
CSMA/CA:wireless,偵測他是不是空的.等一下.再偵測一次.把錯誤機率變p^2.再錯就沒救了，等偵測錯誤重傳。

無限網路
AP: access point 兩機透過ap位子無限溝通
Wi-Fi: wireless fidelity
IEEE：無限傳輸協定

網路連線工具
connecting compatibles network(前提是一樣的protocols)
Repeater:把bus的訊號增強（把bus增長）
Bridge:過濾訊息，要交給橋另一端的目的的訊息可過，但其他不給過，讓系統能同時處理兩個訊息
Switch:多方向的橋
Connecting incompatible network
Router:連結不同的協定的網路，要拆封包重新整理，通常有無限網路。通常具有防火牆的功能，決定哪類訊息能走那條路。

P2P:當你想下載某物時你也會變成部分的伺服器，讓原始伺服器的負荷降低。
Distributes system: framework .NET

Internet 最初是一個計畫
Domain 一個網址的位子，數據機
Gateway 通常是一個router
ISP(Internet service provider) 如中華電信
ADSL  限制傳入跟傳出的量，讓大多數人能多下載一點
IPv4:現在在用2^32 ~> IPv6 （domain name)
DNS(Domain name server)查位子在哪的機器
Host name （IP)，與domain name（英文）不是一對一 
VoIP: （voice over Internet protocol) 網路通話如Skype 
FTP: file transfer protocol
Telnet: 如：Ptt ，密碼明文傳送

HTTP: hypertext transfer protocol
XML：是一種HTML,純文字，一定要有開頭結尾的tag，把資料包在tag裡就能標準化，不一定要用在網頁。

Client side:javascript flash
Service side:有不想讓你知道的資料
線上遊戲：圖形是client side 數據是service side

Internet protocol 
Application layer：指定要傳的資料，目的
Transport layer:把資料切碎，封包，拼湊得到的
Network layer:傳出去，決定怎麼走(純預測）
Link layer:丟資料

Port: http通常都是80

TCP/IP
 TCP:需要handshaking，reliable,長途通訊
 UTP:不用通知，區網

## 演算法
表示法
Flow chart流程圖，只能表示簡單的
Pseudo code 假的或不標準的程式碼，要有1assignment 2condition 3repeat 4function ，不要從零開始從一開始

解決問題方法
Top down :先分成子問題，逐一解決。
Bottom up:從下往上一個個解決

排序法
Insert sort：n=2，每次排好前n個，n++
Binary search:二分搜，top down

遞迴函數很浩時間，因為要重配記憶體。用loop取代。
費式數列應用：兔子生兔子，爬樓梯的方法。

Divide and conquer(D&C):切割成子問題逐一解決。子問題是independent 的
Dynamic programming (DP):最短路徑、矩陣乘法，子問題overlapping。

演算法效率
時間不行，因為跟機器有關係
演算法極限，重要的是當n極大時n，小時不管
Big-O: 100n=O(n^2)對，因為當n無線大時成立,100n=(n^100)也對
》
Big-omega:n極大時會比較小
》
Big-theta:又是bigO又是big-omega ，乘一個常數後會變上下界。

Best case不重要, worst case, average case.

Const int I=5後面沒法改了

## 程式語言

第一代 機器語言 第二代 組合語言
第三代 高階語言

高階語言特性
1. machine independent 所有機器都可以操作
2.非one to one mapping
3.compiler編譯，編譯第一次後記錄之。interpreter，如口譯，即時編譯，優點是能適應不同機器

程式語言formal language 重視文法

分類
1. Functional 函式導向
f(x y)->(f x y)
2. Object-orientation 如C++, informal hiding & inheritance & abstraction 
3. Imperative 如C,procedural & algorithm
4. Declarative 為問題找到定義

Data structure
Homogeneous 底下的東西一樣大
Heterogeneous 底下的東西不一樣大

Constant: 
Const int a =1 or final a =1
A constant cannot be a l- value 

Function
1. 記錄的是一個地址
2.有自己的local memory stack
3.函式裡頭的變數叫formal，函式呼叫的那個原本的變數叫actual

Call by value 建立一個新的變數，把原變數值丟入
Call by reference 給原變數第二個名字
Call by address 給要操縱原變數大人那個變數地址

Translation process 
1.Lexical analyzer :identify tokens a=b-> a = b
2.parser:建立parser tree 

Strongly typed: no coercion 不允許型態轉變

OOP特性（物件導向）
1.Destructor & garbage collection （自動偵測是否被其他參數引用若無則刪除）
2.Encapsulation資料封包：避免引用者用錯或更改錯誤，設定如：private. Getter & setter 
3.Inheritance:?
4.concurrency:每個物件確保自己一次只被一個東西引用（插紅旗）

Declarative programming 
1.resolution :結合兩個邏輯敘述得到新的關係
2.unification: 用一個邏輯關係得到一個變數的值
3.conjuction normal form 一堆的邏輯關係組成的句子。clause 子句

P and not P 為空集合得到矛盾

**Prolog 某個語言**
1.第一個字大寫是變數，小寫是常數
2.fact: fast (rabbit,turtle)
3. :-表if
4.開始前打 [user].然後打事實，然後control d 
5.如果有別的答案會有？按；可以繼續找下個答案
6.=代表賦值但沒有evaluation,is evaluable 的=   (c裡常用）
7.term==term. Evaluable =:= evaluable
8. 建立檔案*.pl (a pure text file) double click 就會讀進去
9.listing 顯示讀入的檔案
10._底線表示所有都為true 
R




## 資料結構
Dynamic vs static 隨存隨取 與 不能改
靜態二元陣列給的是連續的地址
動態二元陣列，第一個一維陣列記第二個記憶體的地址

Homogeneous array 一個陣列裡元素都一樣大一樣的型態。二維陣列，用一維裡記一維，address polynomial。函式若引入陣列只是給開頭地址過去。

宣告時
`emplate <class T>`
`Void swap(T &a）...`
呼叫時
`Swap<int>(a)`
》也可以用來做class

Stack: first in last out 
Queue : first in first out (FIFO)

Contiguous list: 連續儲存，取得方便，刪除或更改不方便
Linked list ：一個元素除了存的值外會有記憶體指到下一個元素，最後指向null

List. ->??

變化
Dummy node:第一個位子不給值，好處是可以insert 第一個跟第二個的程式會一樣
Circular：把最後一個指到第一個
Double link list: 指向前一個跟後一個，要插最後面不用把全部走完一遍只要往回走。串連兩個list也很方便，因為也要走到最後一個。

Stack :
只有一個stack point ,應用：偵測掛號有沒有少、特殊計算機
Circular:可以加速，當front = rear時如果上一次是pop那就是空的若push就是滿的
Queue :
有兩個access point ,

Tree: 
一個集合有很多元素，有一個最特別的叫root，每個分部都是另一個tre>
適合hierarchical 的資料
Root最上面-> children -> grandchildren -> leaves

Node degree :下面有幾個分支，樹的degree所有node中最大的值

Binary tree : node小於等於2
記憶體中如果很密就用用陣列記1-1，23-2，4567-3...其他用link
》
Binary search tree (BST）
In order 時左邊比你小右邊比妳大（等於）
可以做Traversal:把全部值看一次並做一件事&Search insertion deletion
Big theta =log n(樹的高度）（前提是兩邊平衡）
給pre order 或post order 得到 in order 
Delete a degree-2:找到大於自己的最小值，把自己跟他交換，把在新位子的自己砍掉（一定非degree 2) 

Priority queue 
Binary min heap: complete binary tree最後一層外都要填滿，最後一層要靠左。用array表示。parent小於等於children. Traversal with index.
Push 時，先放在最後一個位子，如果小於parent 跟他換。
Pop把第一個給出去，把最後一個拉到第一個，然後互換。
下沈時找小的換
Big theta = log n （heap search)

［資料庫］
Multidimensional,可以對單一dimension做處理access。

Schema 資料庫的整體設計結構
Subschema:誰可以做什麼

DBMS:應用程式access資料庫的窗口1.避免無法同步的問題2.處理disturbed database

Relational database: 
1.多對多的function ,column= attribute, row= tuple 
2.最好分成多個relation,怎麼分分幾個很難

Operations:
Select: choose row 橫
Project: choose column 直
Join : 合成
！！不同relation的attribute不同

SQL:Structure query language 
Insert, drop,delete, update

## 3D繪圖
平行投影：把z軸丟掉.不真，但還是會保留Z的值.為了要決定誰遮住誰
投射投影：近大遠小,似人類視覺，會失真所以會頭暈因為大腦要處理錯覺

Modeling :通常用三角，因為三點必同平面，好找法向量

Bezier: 會通過第一個點與最後一個點，但其他點對線有影響力，曲線必在多邊形內

Recursive subdivision:
一直切一半，可以趨近於曲線

Lighting :
周圍光：漫射來的、當作最弱的背景光
Directional: 理想的平行光
Point lighting : 點光源，強度與距離有關係

Shading :  
1.每個面分別上色2.找法向量（單位）3.diffusion霧面：找cos內機，直射光最強4.specular鏡面： cos的x次方得到越接近反射角的線上最亮，反射光是光的顏色其他是物體的顏色

內差著色法，可以使有稜有角的東西看起來平滑，比多切三角形還有效率。
Flat shading :每個三角形一個顏色
》點的normal vector是它有碰到的邊的normal vector 的平均，得到三角形頂點的顏色。；
Gouraud shading:每個點都是三角形頂點的內差
Phong shading: 得到的是三角形三頂點的normal vector, 拿向量去外積。運算量最大。

## 人工智能
Act(think) like human(rationally)

Natural language processing 語音判斷
Knowledge representation 知識學習
Automated reasoning建自己的知識架構
Machine learning

Weak AI : exhibit intelligent behavior
Strong A I: possess-consciousness
如： 一個老外在一間有中英對照表的房間，有behavior沒consciousness 

Performance oriented： 做某個動作
Simulation oriented: 像人，研究人怎麼辦到的

Image processing 
Edge enhancement, region finding , smoothing , hough transformation偵測線的存在

Reasoning 知道結果找到開始：
Production systems:
Rule, precondition, initial or goal state, control systems

BFS: 寬度優先
DFS: 深度優先

Perfect information :完美資訊，全部資訊都知道
Zero sum : 一輸一贏，或平手

Heuristic: 
粗估，用search但只估計，可以多search幾層
Minimax search: 
給一個狀態一個分數，走分數最高或最低那步
Alpha-beta Pruning:
找到最小值就不用找其他可能，第一層先heuristic找最大或最小的開始，很容易能pruning 
Forward pruning:
只找合理的步往下走
水平線效應：
當heuristic值變動很大時，要繼續找因為可能下一層（搜不到的）會變很多。

Supervised監督式學習： 
需要classifier分類器，可以給棋譜分數，把資料找出feature 。如：神經網路。
Unsupervised分類式： 
如reinforcement learning(給每一步reward）、evolutionary(隨機產生feature，贏得留下，並產生子代）
類神經網路：neural network線性切割
Association memory: 看到甲想到乙，但觸發的東西不完全是你看到的
黑盒子最佳化： 找到一個函數能夠產出最好的y
1+1 revolution strategy: 產生亂數（常態分佈）
五分之一rule :成功機率高（超過五分之一）時應加大步伐，反之則反之



---
###### tags: `CS`
