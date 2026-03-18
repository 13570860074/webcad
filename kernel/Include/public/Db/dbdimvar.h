#ifndef DBDIMVAR_H
#define DBDIMVAR_H

//
// DESCRIPTION: 
// The api for dimensioning variables shared by 
//  DbDatabase, 
//  DbDimstyleTableRecord, and 
//  DbDimension
    
    //  -------------------------  Dimension Style get methods
    //



    // 标注小数位数
    virtual int              dimadec() const;  // new to R15

    // 控制标注中换算单位的显示,false:禁用换算单位 true:启用换算单位
    virtual bool             dimalt() const;

    // 控制换算单位中小数位的位数
    virtual int              dimaltd() const;

    // 控制换算单位乘数
    virtual double           dimaltf() const;

    // 舍入换算标注单位
    virtual double           dimaltrnd() const;  // new to R15

    // 标注换算单位公差值小数位的位数
    virtual int              dimalttd() const;

    // 控制是否对公差值作消零处理
    virtual int              dimalttz() const;

    // 所有标注样式族(角度标注除外)换算单位格式
    virtual int              dimaltu() const;

    // 控制是否对换算单位标注值作消零处理。DIMALTZ 值为 0 - 3 时只影响英尺-英寸标注。
    virtual int              dimaltz() const;

    // 标注(角度标注除外)的换算标注测量值指定文字前缀或后缀(或两者都指定)
    virtual const char* dimapost() const;

    virtual int              dimarcsym() const;  // new to 2007

    // 获得箭头长度
    virtual double           dimasz() const;

    // 当尺寸界线的空间不足以同时放下标注文字和箭头时,该变量将确定这两者的排列方式
    virtual int              dimatfit() const;  // new to R15

    // 角度标注的单位格式：0．十进制度数 1．度/分/秒 2．百分度 3．弧度
    virtual int              dimaunit() const;

    // 角度标注的消零处理
    virtual int              dimazin() const;  // new to R15

    // 获得尺寸线或引线末端显示的箭头块
    virtual DbObjectId     dimblk() const;

    // 获得尺寸线第一个端点的箭头(当DIMSAH系统变量打开时有效)
    virtual DbObjectId     dimblk1() const;

    // 获得尺寸线第二个端点的箭头(当DIMSAH系统变量打开时有效)
    virtual DbObjectId     dimblk2() const;

    // (未知)
    virtual double           dimcen() const;

    // 获得尺寸线,箭头和标注引线的颜色
    virtual CmColor        dimclrd() const;

    // 获得尺寸界线的颜色
    virtual CmColor        dimclre() const;

    // 获得标注文字的颜色
    virtual CmColor        dimclrt() const;

    // 获得主单位的小数精度(0-8)
    virtual int              dimdec() const;

    // 获得尺寸线超出尺寸界线的距离(当使用小斜线代替箭头进行标注时)
    virtual double           dimdle() const;

    // 获得基线标注中尺寸线的间距
    virtual double           dimdli() const;

    // 获得十进制标注时使用的小数分隔符
    virtual char            dimdsep() const;  // new to R15

    // 获得尺寸界线超出尺寸线的距离
    virtual double           dimexe() const;

    // 获得尺寸界线偏移原点的距离
    virtual double           dimexo() const;

    // 获得分数格式(0．水平 1．斜 2．不堆叠),DIMLUNIT值为：4（建筑）或 5（分数）时生效
    virtual int              dimfrac() const;  // new to R15

    // 获得标注文字周围的距离,当尺寸线分成段以在两段之间放置标注文字时生效
    virtual double           dimgap() const;

    virtual double           dimjogang() const;  // new to 2007

    // 获得标注文字的水平位置
    virtual int              dimjust() const;

    // 获得引线箭头的类型
    virtual DbObjectId     dimldrblk() const;  // new to R15

    // 获得线性标注测量值的比例因子
    virtual double           dimlfac() const;

    // 获得是否将极限尺寸生成为默认文字
    virtual bool             dimlim() const;

    virtual DbObjectId     dimltex1() const;  // new to 2007
    virtual DbObjectId     dimltex2() const;  // new to 2007
    virtual DbObjectId     dimltype() const;  // new to 2007

    // 获得为所有标注(除角度标注外)单位制
    virtual int              dimlunit() const;  // new to R15

    // 获得尺寸线的线宽。其值是标准线宽。-3．BYLAYER -2．BYBLOCK 整数代表百分之一毫米的倍数
    virtual Db::LineWeight dimlwd() const;  // new to R15

    // 获得尺寸界线的线宽。其值是标准线宽。-3 BYLAYER -2 BYBLOCK 整数代表百分之一毫米的倍数
    virtual Db::LineWeight dimlwe() const;  // new to R15

    // 获得标注测量值的文字前缀或后缀
    virtual const char* dimpost() const;

    // 获得舍入值
    virtual double           dimrnd() const;

    // 获得尺寸线箭头块是否显示
    virtual bool             dimsah() const;

    // 获得标注变量(指定尺寸、距离或偏移量)的全局比例因子.同时还影响 LEADER 命令创建的引线对象的比例
    virtual double           dimscale() const;

    // 获得是否禁止显示第一条尺寸线
    virtual bool             dimsd1() const;

    // 获得是否禁止显示第二条尺寸线
    virtual bool             dimsd2() const;

    // 获得是否禁止显示第一条尺寸界线
    virtual bool             dimse1() const;

    // 获得是否禁止显示第二条尺寸界线
    virtual bool             dimse2() const;

    // 获得是否允许尺寸线绘制到尺寸界线之外
    virtual bool             dimsoxd() const;

    // 获得文字相对尺寸线的垂直位置
    virtual int              dimtad() const;

    // 获得标注主单位的公差值显示的小数位位数
    virtual int              dimtdec() const;

    // 获得相对于标注文字高度给分数值和公差值的文字高度指定比例因子
    virtual double           dimtfac() const;

    virtual int              dimtfill() const;   // new to 2007

    // (未知)
    virtual CmColor        dimtfillclr() const; // new to 2007

    // 获得所有标注类型(坐标标注除外)的标注文字在尺寸界线内的位置
    virtual bool             dimtih() const;

    // 获得是否在尺寸界线之间绘制文字
    virtual bool             dimtix() const;

    // 获得标注文字设置最小(下)偏差,在 DIMTOL 或 DIMLIM 变量为开的情况下
    virtual double           dimtm() const;

    // 获得标注文字的移动规则
    virtual int              dimtmove() const;  // new to R15

    // 获得是否将尺寸线绘制在尺寸界线之间(即使文字放置在尺寸界线之外)
    virtual bool             dimtofl() const;

    // 获得标注文字在尺寸界线外的位置,false 表示文字与尺寸线对齐, true表示水平绘制文字
    virtual bool             dimtoh() const;

    // 获得是否将公差附在标注文字之后(将 DIMTOL 设置为true，将关闭 DIMLIM 系统变量)
    virtual bool             dimtol() const;

    // 获得公差值相对名词性标注文字的垂直对正方式：0．下 1．中间 2．上
    virtual int              dimtolj() const;

    // 获得接受带符号的值
    virtual double           dimtp() const;

    // 获得线性标注,半径标注以及直径标注中替代箭头的小斜线尺寸
    virtual double           dimtsz() const;

    // 获得尺寸线上方或下方标注文字的垂直位置。当 DIMTAD 设置为false时, 将使用 DIMTVP 的值
    virtual double           dimtvp() const;

    // 获得标注的文字样式
    virtual DbObjectId     dimtxsty() const;

    // 获得标注文字的高度,除非当前文字样式具有固定的高度
    virtual double           dimtxt() const;

    // 获得公差的消零处理
    virtual int              dimtzin() const;

    // 获得用户定位文字的选项。0光标仅控制尺寸线的位置1或开光标控制文字以及尺寸线的位置
    virtual bool             dimupt() const;

    // 获得主单位的消零处理
    virtual int              dimzin() const;

    // (未知)
    virtual bool             dimfxlenOn() const;  // new to R2007

    // (未知)
    virtual double           dimfxlen() const;  // new to R2007

    // (未知)
    virtual bool             dimtxtdirection() const;  // new to 2010

    // (未知)
    virtual double           dimmzf() const;        // new to 2010

    // (未知)
    virtual const char* dimmzs() const;        // new to 2010

    // (未知)
    virtual double           dimaltmzf() const;        // new to 2010

    // (未知)
    virtual const char* dimaltmzs() const;        // new to 2010

    //  -------------------------  Dimension Style set methods
    //
    // 设置标注小数位数
    virtual Acad::ErrorStatus setDimadec(int              v);  // new to R15

    // 设置控制标注中换算单位的显示,false:禁用换算单位 true:启用换算单位
    virtual Acad::ErrorStatus setDimalt(bool             v);

    // 设置控制换算单位中小数位的位数
    virtual Acad::ErrorStatus setDimaltd(int              v);

    // 设置控制换算单位乘数
    virtual Acad::ErrorStatus setDimaltf(double           v);

    virtual Acad::ErrorStatus setDimaltmzf(double           v);  // new to 2010

    // (未知)
    virtual Acad::ErrorStatus setDimaltmzs(const char* v);  // new to 2010

    // 设置舍入换算标注单位
    virtual Acad::ErrorStatus setDimaltrnd(double           v);  // new to R15

    // 设置标注换算单位公差值小数位的位数
    virtual Acad::ErrorStatus setDimalttd(int              v);

    // 设置控制是否对公差值作消零处理
    virtual Acad::ErrorStatus setDimalttz(int              v);

    // 设置所有标注样式族(角度标注除外)换算单位格式
    virtual Acad::ErrorStatus setDimaltu(int              v);

    // 设置是否对换算单位标注值作消零处理。DIMALTZ 值为 0 - 3 时只影响英尺-英寸标注。
    virtual Acad::ErrorStatus setDimaltz(int              v);

    // 设置标注(角度标注除外)的换算标注测量值文字前缀或后缀(或两者都指定)
    virtual Acad::ErrorStatus setDimapost(const char* v);

    // (未知)
    virtual Acad::ErrorStatus setDimarcsym(int              v);  // new to 2007

    // 设置箭头长度
    virtual Acad::ErrorStatus setDimasz(double           v);

    // 设置当尺寸界线的空间不足以同时放下标注文字和箭头时,该变量将确定这两者的排列方式
    virtual Acad::ErrorStatus setDimatfit(int              v);  // new to R15

    // 设置角度标注的单位格式：0．十进制度数 1．度/分/秒 2．百分度 3．弧度
    virtual Acad::ErrorStatus setDimaunit(int              v);

    // 设置角度标注的消零处理
    virtual Acad::ErrorStatus setDimazin(int              v);  // new to R15

    // 设置尺寸线或引线末端显示的箭头块
    virtual Acad::ErrorStatus setDimblk(DbObjectId     v);  // new to R15

    // 设置尺寸线第一个端点的箭头(当DIMSAH系统变量打开时有效)
    virtual Acad::ErrorStatus setDimblk1(DbObjectId     v);  // new to R15

    // (未知)
    virtual Acad::ErrorStatus setDimblk2(DbObjectId     v);  // new to R15

    // 未知
    virtual Acad::ErrorStatus setDimcen(double           v);

    // 设置尺寸线,箭头和标注引线指定颜色。同时控制由 LEADER 命令创建的引线颜色
    virtual Acad::ErrorStatus setDimclrd(const CmColor& v);

    // 设置尺寸界线的颜色
    virtual Acad::ErrorStatus setDimclre(const CmColor& v);

    // 设置标注文字的颜色
    virtual Acad::ErrorStatus setDimclrt(const CmColor& v);

    // 设置主单位的小数精度(0-8)
    virtual Acad::ErrorStatus setDimdec(int              v);

    // 设置尺寸线超出尺寸界线的距离(当使用小斜线代替箭头进行标注时)
    virtual Acad::ErrorStatus setDimdle(double           v);

    // 设置基线标注中尺寸线的间距
    virtual Acad::ErrorStatus setDimdli(double           v);

    // 设置十进制标注时使用的小数分隔符
    virtual Acad::ErrorStatus setDimdsep(char            v);  // new to R15

    // 设置尺寸界线超出尺寸线的距离
    virtual Acad::ErrorStatus setDimexe(double           v);

    // 设置尺寸界线偏移原点的距离
    virtual Acad::ErrorStatus setDimexo(double           v);

    // 设置分数格式(0．水平 1．斜 2．不堆叠),DIMLUNIT值为：4（建筑）或 5（分数）时生效
    virtual Acad::ErrorStatus setDimfrac(int              v);  // new to R15

    // 设置标注文字周围的距离,当尺寸线分成段以在两段之间放置标注文字时生效
    virtual Acad::ErrorStatus setDimgap(double           v);

    virtual Acad::ErrorStatus setDimjogang(double           v);  // new to 2007

    // 设置标注文字的水平位置
    virtual Acad::ErrorStatus setDimjust(int              v);

    virtual Acad::ErrorStatus setDimldrblk(DbObjectId     v);  // new to R15

    // 设置线性标注测量值的比例因子
    virtual Acad::ErrorStatus setDimlfac(double           v);

    // 设置是否将极限尺寸生成为默认文字
    virtual Acad::ErrorStatus setDimlim(bool             v);

    // (未知)
    virtual Acad::ErrorStatus setDimltex1(DbObjectId     v);  // new to 2007

    // (未知)
    virtual Acad::ErrorStatus setDimltex2(DbObjectId     v);  // new to 2007

    // (未知)
    virtual Acad::ErrorStatus setDimltype(DbObjectId     v);  // new to 2007

    // 设置所有标注(除角度标注外)单位制(new to R15)
    virtual Acad::ErrorStatus setDimlunit(int              v);  // new to R15

    // 设置尺寸线的线宽。其值是标准线宽。-3．BYLAYER -2．BYBLOCK 整数代表百分之一毫米的倍数(new to R15)
    virtual Acad::ErrorStatus setDimlwd(Db::LineWeight v);  // new to R15

    //设置尺寸界线的线宽。其值是标准线宽。-3 BYLAYER -2 BYBLOCK 整数代表百分之一毫米的倍数(new to R15)
    virtual Acad::ErrorStatus setDimlwe(Db::LineWeight v);  // new to R15

    // (未知)
    virtual Acad::ErrorStatus setDimmzf(double           v);  // new to 2010

    // (未知)
    virtual Acad::ErrorStatus setDimmzs(const char* v);  // new to 2010

    // 设置标注测量值的文字前缀或后缀(或者两者都指定)
    virtual Acad::ErrorStatus setDimpost(const char* v);

    // 设置舍入值
    virtual Acad::ErrorStatus setDimrnd(double           v);

    // 设置尺寸线箭头块是否显示
    virtual Acad::ErrorStatus setDimsah(bool             v);

    // 设置标注变量(指定尺寸、距离或偏移量)的全局比例因子.同时还影响 LEADER 命令创建的引线对象的比例
    virtual Acad::ErrorStatus setDimscale(double           v);

    // 设置是否禁止显示第一条尺寸线
    virtual Acad::ErrorStatus setDimsd1(bool             v);

    // 设置是否禁止显示第二条尺寸线
    virtual Acad::ErrorStatus setDimsd2(bool             v);

    // 设置是否禁止显示第一条尺寸界线
    virtual Acad::ErrorStatus setDimse1(bool             v);

    // 设置是否禁止显示第二条尺寸界线
    virtual Acad::ErrorStatus setDimse2(bool             v);

    // 设置是否允许尺寸线绘制到尺寸界线之外
    virtual Acad::ErrorStatus setDimsoxd(bool             v);

    // 设置文字相对尺寸线的垂直位置
    virtual Acad::ErrorStatus setDimtad(int              v);

    // 设置标注主单位的公差值显示的小数位位数
    virtual Acad::ErrorStatus setDimtdec(int              v);

    // 设置相对于标注文字高度给分数值和公差值的文字高度指定比例因子
    virtual Acad::ErrorStatus setDimtfac(double           v);

    // (未知)
    virtual Acad::ErrorStatus setDimtfill(int              v);   // new to 2007

    // (未知)
    virtual Acad::ErrorStatus setDimtfillclr(const CmColor& v); // new to 2007

    // 设置所有标注类型(坐标标注除外)的标注文字在尺寸界线内的位置
    virtual Acad::ErrorStatus setDimtih(bool             v);

    // 设置是否在尺寸界线之间绘制文字
    virtual Acad::ErrorStatus setDimtix(bool             v);

    // 设置标注文字设置最小(下)偏差,在 DIMTOL 或 DIMLIM 变量为开的情况下
    virtual Acad::ErrorStatus setDimtm(double           v);

    // 设置标注文字的移动规则(new to R15)
    virtual Acad::ErrorStatus setDimtmove(int              v);  // new to R15

    // 设置是否将尺寸线绘制在尺寸界线之间(即使文字放置在尺寸界线之外)
    virtual Acad::ErrorStatus setDimtofl(bool             v);

    // 设置标注文字在尺寸界线外的位置,false 表示文字与尺寸线对齐, true表示水平绘制文字
    virtual Acad::ErrorStatus setDimtoh(bool             v);

    // 设置是否将公差附在标注文字之后(将 DIMTOL 设置为true，将关闭 DIMLIM 系统变量)
    virtual Acad::ErrorStatus setDimtol(bool             v);

    // 设置公差值相对名词性标注文字的垂直对正方式：0．下 1．中间 2．上
    virtual Acad::ErrorStatus setDimtolj(int              v);

    // 设置接受带符号的值
    virtual Acad::ErrorStatus setDimtp(double           v);

    // 设置线性标注,半径标注以及直径标注中替代箭头的小斜线尺寸
    virtual Acad::ErrorStatus setDimtsz(double           v);

    // //设置尺寸线上方或下方标注文字的垂直位置。当 DIMTAD 设置为false时, 将使用 DIMTVP 的值
    virtual Acad::ErrorStatus setDimtvp(double           v);

    // 设置标注的文字样式
    virtual Acad::ErrorStatus setDimtxsty(DbObjectId     v);

    // 设置标注文字的高度,除非当前文字样式具有固定的高度
    virtual Acad::ErrorStatus setDimtxt(double           v);

    // new 2010
    virtual Acad::ErrorStatus setDimtxtdirection(bool      v);

    // 设置公差的消零处理
    virtual Acad::ErrorStatus setDimtzin(int              v);

    // 设置用户定位文字的选项。0光标仅控制尺寸线的位置1或开光标控制文字以及尺寸线的位置
    virtual Acad::ErrorStatus setDimupt(bool             v);

    // 设置主单位的消零处理
    virtual Acad::ErrorStatus setDimzin(int              v);

    // (未知)
    virtual Acad::ErrorStatus setDimblk(const char* v);

    // (未知)
    virtual Acad::ErrorStatus setDimblk1(const char* v);

    // 设置尺寸线第二个端点的箭头(当DIMSAH系统变量打开时有效)
    virtual Acad::ErrorStatus setDimblk2(const char* v);

    // 设置引线箭头的类型
    virtual Acad::ErrorStatus setDimldrblk(const char* v);  // new to R15

    // (未知)
    virtual Acad::ErrorStatus setDimfxlenOn(bool           v);  // new to R2007

    // (未知)
    virtual Acad::ErrorStatus setDimfxlen(double           v);  // new to R2007


#endif

