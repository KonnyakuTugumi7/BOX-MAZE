#pragma once

#pragma warning( disable : 4244 )

//------------------補間キー値テンプレート------------------
template< class VAL , class X = double , class W = double >
class CTrio{
public:
	//値
	VAL m_val;

	//補間距離
	X m_x;

	//補間ウェイト
	W m_w;

public:
	//コンストラクタ
	CTrio( VAL& val , X& x , W& w ) : m_val( val ) , m_x( x ) , m_w( w )
	{

	}

};

//----------------------補間ストラテジ基本クラス----------------------
template< class VAL , class X = double , class W = double >
class CInterpBase{
	//型再定義
	typedef typename CTrio< VAL , X , W > TRI;

public:
	//必要な点の数を取得
	virtual size_t NeedPointNum() = 0;

	//補間値を計算
	virtual bool Calc( VAL* pOut , X& t ) = 0;

	//点を登録する配列を取得
	virtual TRI** GetPtrAry() = 0;

};

//-----------------線形補間ストラテジクラス-----------------
template< class VAL , class X = double , class W = double >
class CLinerInterp : public CInterpBase< VAL , X , W >{
	//型再定義
	typedef typename CTrio< VAL , X , W > TRI;

protected:
	//補間キー値
	TRI* Points[ 2 ];

public:
	//コンストラクタ
	CLinerInterp() : Points[ 0 ]( NULL ) , Points[ 1 ]( NULL )
	{

	}

	//必要な点の数を取得
	inline virtual size_t NeedPointNum()
	{
		//線形補間は2つの点が必要
		return 2;
	}

	//補間値を計算
	inline virtual bool Calc( VAL* pOut , X& t )
	{
		//線形補間値を計算
		*pOut = ( 1 - t ) * ( Points[ 0 ]-> _val ) + ( Points[ 1 ]-> _val ) * t;

		return true;
	}

	//点を登録する配列を取得
	inline virtual TRI** GetPtrAry()
	{
		return ( TRI** )&Points;
	}

};

//終端ポインタマクロ
#define INTERPOLATING_ENDPTR ( ( TRI* )( -1 ) )

double P1 = 0;
double P2 = 50;
double P3 = 20;

//CInterpolating< double > ip;
//double x;
////補間単位
//double unit = 0.5;
//ip.push_back( P1 , x = 0 );
//ip.push_back( P2 , x = 10 );
//ip.push_back( P3 , x = 20 );
//
////補間を実行してファイル出力
//interpolating< double >::iterator it;
//for( it = ip.begin() ; it != ip.end() ; it++ ){
//	//補間単位を0.5に
//	it.SetUnit( unit );
//	double p = ( *it );
//	ofs << it.getDist() << "\t"
//		<< p.m_x << "\t"
//		<< p.m_y << endl;
//}

//-----------------------------------------補間テンプレートクラス-----------------------------------------
//任意の型の変数(補間する値)と補間距離(始点の補間距離は無視される)をpush_back(線形補間なので最低２つの変数(AとB)を入れる)してiteratorで補間単位を設定し回す
//(AからBへの距離) / (補間距離) * 補間単位
//↑が実際の補間１フレーム当たりのの値となる↑
template< class VAL , class X = double , class W = double , class STRATEGY = CLinerInterp< VAL , X , W > >
class CInterpolating{
	//型再定義
	typedef typename CTrio< VAL , X , W > TRI;

public:
	//----------イテレータ(内包クラス)----------
	class CIterator{
	private:
		//イテレートの最初のポインタ位置
		TRI **m_ppFirst;

		//アクセス元のCInterpolatingオブジェクト
		CInterpolating *m_pInterp;

		//現在の補間値
		VAL m_Val;

		//現在の距離
		X m_cx;

		//累積距離
		X m_Cum;

		//単位
		X m_Unit;

		//オフセット値
		size_t m_Offset;

		//最大格納数
		size_t m_Max;

		//現在の位置
		size_t m_CurPos;

		//補間計算方法
		STRATEGY m_Strategy;

		//現在の点へのポインタ
		TRI *m_pCur;

		//終了済みフラグ
		bool m_Finished;

	public:
		//------------演算子のオーバーロード------------
		//間接指定演算子
		VAL& operator *()
		{
			return m_Val;
		}

		//増加単項演算子
		void operator ++( int dumy )
		{
			//次のイテレータに更新する
			next( m_Unit );
		}

		//等号比較演算子(==演算子はあまり意味を持たない)
		bool operator ==( CIterator& src )
		{
			//指しているポインタが同じ
			if( src.m_pCur == m_pCur ){
				return true;
			}else{
				return false;
			}
		}

		//不等号比較演算子
		bool operator !=( CIterator& src )
		{
			//指しているポインタが違う
			if( src.m_pCur != m_pCur ){
				return true;
			}else{
				return false;
			}
		}

		//コンストラクタ
		CIterator( TRI** ppTrio = NULL , CInterpolating* pInterp = NULL )
		{
			//オフセット値
			m_Offset = 0;

			//最大格納数
			m_Max = 0;

			//終了済みフラグ(終端に達している状態)
			m_Finished = true;

			//単位(1単位に初期化)
			m_Unit = 1;

			//現在の距離(初期距離)
			m_cx = 0;

			//累積距離(距離を初期化)
			m_Cum = 0;

			//イテレートの最初のポインタ位置
			m_ppFirst = ppTrio;

			//アクセス元のCInterpolatingオブジェクト
			m_pInterp = pInterp;

			//現在の位置(補間出来ない状態)
			m_CurPos = 0;

			//現在の点へのポインタ(終端ポインタにする)
			m_pCur = INTERPOLATING_ENDPTR;

			//配列の有効性をチェック
			if( ppTrio != NULL ){
				//オフセット数、最大更新数（＝点の数）、現在の点の位置を初期化
				//先頭からのオフセット数
				m_Offset = ( ( size_t )ppTrio - ( size_t )pInterp->GetFirstPtr() ) / sizeof( TRI* );

				//最大更新数
				m_Max = pInterp->size() - m_Offset;

				//初期値
				m_Val = ppTrio[ 0 ]->m_val;

				//点の個数が2つ以上であればとりあえず補間可能
				if( m_Max >= 2 ){
					//最初の目標点は2点目にあるので1にする
					m_CurPos = 1;

					//2点目にする
					m_pCur = m_ppFirst[ m_CurPos ];

					//終端に達していない状態に変更
					m_Finished = false;
				}
			}
		}

		//デストラクタ
		virtual ~CIterator()
		{

		}

		//現在の距離を取得
		X getDist()
		{
			return m_Cum;
		}

		//距離単位を設定
		void SetUnit( X& x )
		{ 
			m_Unit = x;
		}

		//現在の値を取得
		VAL& get()
		{
			return m_Val;
		}

		//イテレータの更新
		CIterator next( X& x )
		{
			//イテレータとしての更新は距離を単位分進める事、補間計算に必要な点を用意する事
			//次の距離へ進め、標準化距離を定義
			X norm = Update( x );

			//補間計算に必要な点の数を取得
			size_t pn = m_Strategy.NeedPointNum();

			//指定の点の確保
			//補間点ポインタを格納する配列
			TRI** _ppPoint = _Strategy.GetPtrAry();
			if( !SetPoint( pn , _ppPoint ) ){
				//最終フラグが立っていたら終端イテレータを返す
				if( m_Finished || *m_ppFirst==INTERPOLATING_ENDPTR ){
					//すでに終了している状態
					//終端イテレータに設定
					SetTarminate();
					return *this;
				}else{
					//最後の点に合わせる
					m_Finished = true;
					m_pCur =m_ppFirst[ m_pInterp->size() -1 ]; 
					m_Val = m_pCur->m_val;
					return *this;
				}
			}

			//次の値に更新
			m_Strategy.Calc( &m_Val, norm);
			return *this;
		}

	protected:
		//終端イテレータの状態にする
		void SetTarminate()
		{
			//終端状態は現在のポインタが終端ポインタになる
			m_pCur = INTERPOLATING_ENDPTR;
		}

		//計算に必要な点を確保
		bool SetPoint( size_t num , TRI** ary )
		{
			size_t LastPointNum = ( m_CurPos - 1 ) + num;
			if( LastPointNum > m_Max ){
				//必要な点を確保出来ない
				return false;
			}else{
				//前の点を基点に点をコピー
				memcpy( ary , m_ppFirst + m_CurPos - 1 , num * sizeof( TRI* ) );
				return true;
			}
		}

		//次の位置と標準化距離を算出
		X Update( X x )
		{
			if( *m_ppFirst == INTERPOLATING_ENDPTR ){
				return 1;
			}

			//1点では補間できない
			if( m_pInterp->size() <= 1 ){
				//最終点を指すようにする
				return 1;  
			}

			//次の位置へ移動
			m_cx += x;
			m_Cum += x;

			//次の距離の所属区間を確定
			while( 1 )
			{
				//今の位置が存在するかチェック
				//存在しない
				if( m_CurPos >= m_Max ){
					return 1;
				}

				//今の位置を通り過ぎていたら次の点へ
				if( m_cx >= m_ppFirst[ m_Offset + m_CurPos ]->m_x ){
					//飛び出た差分距離を算出
					m_cx -= m_ppFirst[ m_Offset + _CurPos ]->m_x;
					m_CurPos++;
					continue;
				}
				break;
			}

			//↑-- 現在の区間位置が確定 --↑//
			m_pCur = m_ppFirst[ m_Offset + m_CurPos ];

			//次の点に対する標準化距離を算出
			return m_cx / m_pCur->m_x;
		}

	};
	typedef typename CInterpolating< VAL , X >::CIterator iterator;

protected:
	//格納数
	size_t m_Size;

	//補間値ポインタ配列(先端)
	TRI **m_ppTrio;

	//ポインタ配列の最後端
	TRI **m_pLast;

public:
	//コンストラクタ
	CInterpolating()
	{
		//格納数
		m_Size = 0;

		//補間値ポインタ配列(先端) //CTrioポインタ格納分確保
		m_ppTrio = ( TRI** )malloc( sizeof( TRI* ) );

		//補間値ポインタ配列
		*m_ppTrio = INTERPOLATING_ENDPTR;

		//ポインタ配列の最後端
		m_pLast  = m_ppTrio;
	}

	//デストラクタ
	virtual ~CInterpolating()
	{
		//開放
		clear();
		free( m_ppTrio );
	}

	//登録
	void push_back( VAL& val , X& x , W weight = 1.0 )
	{
		//トリオを生成
		TRI *p = new TRI( val , x , weight );

		//配列を1つ拡張
		AddAryMemory( 1 );

		//追加した配列部分に値を格納(ポインタの上書き)
		Rewrite( m_pLast , p );
	}

	//格納数取得
	size_t size()
	{
		return m_Size;
	}

	//開始イテレータ取得
	iterator begin()
	{
		return iterator( m_ppTrio , this );
	}

	//終端イテレータを取得
	iterator end()
	{
		return iterator( NULL , this );
	}

protected: 
	//配列の拡張
	void AddAryMemory( size_t Cont )
	{
		if( Cont > 0 ){
			//指定の大きさにポインタ配列を拡張
			m_ppTrio = ( TRI** )realloc( m_ppTrio , sizeof( TRI* ) * ( m_Size + Cont ) );

			//拡張した部分の値を初期化
			for( size_t i = 0 ; i < Cont ; i++ ){
				m_ppTrio[ m_Size + i ] = INTERPOLATING_ENDPTR;
			}

			//サイズを変更
			m_Size += Cont;

			//ポインタの設定を変更
			m_pLast = m_ppTrio + m_Size - 1;
		}
	}

	//ポインタの上書き
	void Rewrite( TRI** pPlace , TRI* val )
	{
		//上書き部分のポインタ先を削除
		if( ( *pPlace ) != INTERPOLATING_ENDPTR ){
			delete *pPlace;
		}

		*pPlace = val;
	}

	//開放
	void clear()
	{
		//確保したメモリを全てクリアする
		for( size_t i = 0 ; i < _Size ; i++){
			delete m_ppTrio[ i ];
		}
		m_Size = 0;
		m_ppTrio = ( TRI** )realloc( m_ppTrio , sizeof( TRI* ) );
		*m_ppTrio = INTERPOLATING_ENDPTR;
		m_pLast  = m_ppTrio;
	}

	// 最初のポインタ位置を取得
	TRI** GetFirstPtr()
	{
		return m_ppTrio;
	}
};