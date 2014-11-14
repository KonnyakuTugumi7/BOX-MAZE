#pragma once

#pragma warning( disable : 4244 )

//------------------��ԃL�[�l�e���v���[�g------------------
template< class VAL , class X = double , class W = double >
class CTrio{
public:
	//�l
	VAL m_val;

	//��ԋ���
	X m_x;

	//��ԃE�F�C�g
	W m_w;

public:
	//�R���X�g���N�^
	CTrio( VAL& val , X& x , W& w ) : m_val( val ) , m_x( x ) , m_w( w )
	{

	}

};

//----------------------��ԃX�g���e�W��{�N���X----------------------
template< class VAL , class X = double , class W = double >
class CInterpBase{
	//�^�Ē�`
	typedef typename CTrio< VAL , X , W > TRI;

public:
	//�K�v�ȓ_�̐����擾
	virtual size_t NeedPointNum() = 0;

	//��Ԓl���v�Z
	virtual bool Calc( VAL* pOut , X& t ) = 0;

	//�_��o�^����z����擾
	virtual TRI** GetPtrAry() = 0;

};

//-----------------���`��ԃX�g���e�W�N���X-----------------
template< class VAL , class X = double , class W = double >
class CLinerInterp : public CInterpBase< VAL , X , W >{
	//�^�Ē�`
	typedef typename CTrio< VAL , X , W > TRI;

protected:
	//��ԃL�[�l
	TRI* Points[ 2 ];

public:
	//�R���X�g���N�^
	CLinerInterp() : Points[ 0 ]( NULL ) , Points[ 1 ]( NULL )
	{

	}

	//�K�v�ȓ_�̐����擾
	inline virtual size_t NeedPointNum()
	{
		//���`��Ԃ�2�̓_���K�v
		return 2;
	}

	//��Ԓl���v�Z
	inline virtual bool Calc( VAL* pOut , X& t )
	{
		//���`��Ԓl���v�Z
		*pOut = ( 1 - t ) * ( Points[ 0 ]-> _val ) + ( Points[ 1 ]-> _val ) * t;

		return true;
	}

	//�_��o�^����z����擾
	inline virtual TRI** GetPtrAry()
	{
		return ( TRI** )&Points;
	}

};

//�I�[�|�C���^�}�N��
#define INTERPOLATING_ENDPTR ( ( TRI* )( -1 ) )

double P1 = 0;
double P2 = 50;
double P3 = 20;

//CInterpolating< double > ip;
//double x;
////��ԒP��
//double unit = 0.5;
//ip.push_back( P1 , x = 0 );
//ip.push_back( P2 , x = 10 );
//ip.push_back( P3 , x = 20 );
//
////��Ԃ����s���ăt�@�C���o��
//interpolating< double >::iterator it;
//for( it = ip.begin() ; it != ip.end() ; it++ ){
//	//��ԒP�ʂ�0.5��
//	it.SetUnit( unit );
//	double p = ( *it );
//	ofs << it.getDist() << "\t"
//		<< p.m_x << "\t"
//		<< p.m_y << endl;
//}

//-----------------------------------------��ԃe���v���[�g�N���X-----------------------------------------
//�C�ӂ̌^�̕ϐ�(��Ԃ���l)�ƕ�ԋ���(�n�_�̕�ԋ����͖��������)��push_back(���`��ԂȂ̂ōŒ�Q�̕ϐ�(A��B)������)����iterator�ŕ�ԒP�ʂ�ݒ肵��
//(A����B�ւ̋���) / (��ԋ���) * ��ԒP��
//�������ۂ̕�ԂP�t���[��������̂̒l�ƂȂ遪
template< class VAL , class X = double , class W = double , class STRATEGY = CLinerInterp< VAL , X , W > >
class CInterpolating{
	//�^�Ē�`
	typedef typename CTrio< VAL , X , W > TRI;

public:
	//----------�C�e���[�^(����N���X)----------
	class CIterator{
	private:
		//�C�e���[�g�̍ŏ��̃|�C���^�ʒu
		TRI **m_ppFirst;

		//�A�N�Z�X����CInterpolating�I�u�W�F�N�g
		CInterpolating *m_pInterp;

		//���݂̕�Ԓl
		VAL m_Val;

		//���݂̋���
		X m_cx;

		//�ݐϋ���
		X m_Cum;

		//�P��
		X m_Unit;

		//�I�t�Z�b�g�l
		size_t m_Offset;

		//�ő�i�[��
		size_t m_Max;

		//���݂̈ʒu
		size_t m_CurPos;

		//��Ԍv�Z���@
		STRATEGY m_Strategy;

		//���݂̓_�ւ̃|�C���^
		TRI *m_pCur;

		//�I���ς݃t���O
		bool m_Finished;

	public:
		//------------���Z�q�̃I�[�o�[���[�h------------
		//�Ԑڎw�艉�Z�q
		VAL& operator *()
		{
			return m_Val;
		}

		//�����P�����Z�q
		void operator ++( int dumy )
		{
			//���̃C�e���[�^�ɍX�V����
			next( m_Unit );
		}

		//������r���Z�q(==���Z�q�͂��܂�Ӗ��������Ȃ�)
		bool operator ==( CIterator& src )
		{
			//�w���Ă���|�C���^������
			if( src.m_pCur == m_pCur ){
				return true;
			}else{
				return false;
			}
		}

		//�s������r���Z�q
		bool operator !=( CIterator& src )
		{
			//�w���Ă���|�C���^���Ⴄ
			if( src.m_pCur != m_pCur ){
				return true;
			}else{
				return false;
			}
		}

		//�R���X�g���N�^
		CIterator( TRI** ppTrio = NULL , CInterpolating* pInterp = NULL )
		{
			//�I�t�Z�b�g�l
			m_Offset = 0;

			//�ő�i�[��
			m_Max = 0;

			//�I���ς݃t���O(�I�[�ɒB���Ă�����)
			m_Finished = true;

			//�P��(1�P�ʂɏ�����)
			m_Unit = 1;

			//���݂̋���(��������)
			m_cx = 0;

			//�ݐϋ���(������������)
			m_Cum = 0;

			//�C�e���[�g�̍ŏ��̃|�C���^�ʒu
			m_ppFirst = ppTrio;

			//�A�N�Z�X����CInterpolating�I�u�W�F�N�g
			m_pInterp = pInterp;

			//���݂̈ʒu(��ԏo���Ȃ����)
			m_CurPos = 0;

			//���݂̓_�ւ̃|�C���^(�I�[�|�C���^�ɂ���)
			m_pCur = INTERPOLATING_ENDPTR;

			//�z��̗L�������`�F�b�N
			if( ppTrio != NULL ){
				//�I�t�Z�b�g���A�ő�X�V���i���_�̐��j�A���݂̓_�̈ʒu��������
				//�擪����̃I�t�Z�b�g��
				m_Offset = ( ( size_t )ppTrio - ( size_t )pInterp->GetFirstPtr() ) / sizeof( TRI* );

				//�ő�X�V��
				m_Max = pInterp->size() - m_Offset;

				//�����l
				m_Val = ppTrio[ 0 ]->m_val;

				//�_�̌���2�ȏ�ł���΂Ƃ肠������ԉ\
				if( m_Max >= 2 ){
					//�ŏ��̖ڕW�_��2�_�ڂɂ���̂�1�ɂ���
					m_CurPos = 1;

					//2�_�ڂɂ���
					m_pCur = m_ppFirst[ m_CurPos ];

					//�I�[�ɒB���Ă��Ȃ���ԂɕύX
					m_Finished = false;
				}
			}
		}

		//�f�X�g���N�^
		virtual ~CIterator()
		{

		}

		//���݂̋������擾
		X getDist()
		{
			return m_Cum;
		}

		//�����P�ʂ�ݒ�
		void SetUnit( X& x )
		{ 
			m_Unit = x;
		}

		//���݂̒l���擾
		VAL& get()
		{
			return m_Val;
		}

		//�C�e���[�^�̍X�V
		CIterator next( X& x )
		{
			//�C�e���[�^�Ƃ��Ă̍X�V�͋�����P�ʕ��i�߂鎖�A��Ԍv�Z�ɕK�v�ȓ_��p�ӂ��鎖
			//���̋����֐i�߁A�W�����������`
			X norm = Update( x );

			//��Ԍv�Z�ɕK�v�ȓ_�̐����擾
			size_t pn = m_Strategy.NeedPointNum();

			//�w��̓_�̊m��
			//��ԓ_�|�C���^���i�[����z��
			TRI** _ppPoint = _Strategy.GetPtrAry();
			if( !SetPoint( pn , _ppPoint ) ){
				//�ŏI�t���O�������Ă�����I�[�C�e���[�^��Ԃ�
				if( m_Finished || *m_ppFirst==INTERPOLATING_ENDPTR ){
					//���łɏI�����Ă�����
					//�I�[�C�e���[�^�ɐݒ�
					SetTarminate();
					return *this;
				}else{
					//�Ō�̓_�ɍ��킹��
					m_Finished = true;
					m_pCur =m_ppFirst[ m_pInterp->size() -1 ]; 
					m_Val = m_pCur->m_val;
					return *this;
				}
			}

			//���̒l�ɍX�V
			m_Strategy.Calc( &m_Val, norm);
			return *this;
		}

	protected:
		//�I�[�C�e���[�^�̏�Ԃɂ���
		void SetTarminate()
		{
			//�I�[��Ԃ͌��݂̃|�C���^���I�[�|�C���^�ɂȂ�
			m_pCur = INTERPOLATING_ENDPTR;
		}

		//�v�Z�ɕK�v�ȓ_���m��
		bool SetPoint( size_t num , TRI** ary )
		{
			size_t LastPointNum = ( m_CurPos - 1 ) + num;
			if( LastPointNum > m_Max ){
				//�K�v�ȓ_���m�ۏo���Ȃ�
				return false;
			}else{
				//�O�̓_����_�ɓ_���R�s�[
				memcpy( ary , m_ppFirst + m_CurPos - 1 , num * sizeof( TRI* ) );
				return true;
			}
		}

		//���̈ʒu�ƕW�����������Z�o
		X Update( X x )
		{
			if( *m_ppFirst == INTERPOLATING_ENDPTR ){
				return 1;
			}

			//1�_�ł͕�Ԃł��Ȃ�
			if( m_pInterp->size() <= 1 ){
				//�ŏI�_���w���悤�ɂ���
				return 1;  
			}

			//���̈ʒu�ֈړ�
			m_cx += x;
			m_Cum += x;

			//���̋����̏�����Ԃ��m��
			while( 1 )
			{
				//���̈ʒu�����݂��邩�`�F�b�N
				//���݂��Ȃ�
				if( m_CurPos >= m_Max ){
					return 1;
				}

				//���̈ʒu��ʂ�߂��Ă����玟�̓_��
				if( m_cx >= m_ppFirst[ m_Offset + m_CurPos ]->m_x ){
					//��яo�������������Z�o
					m_cx -= m_ppFirst[ m_Offset + _CurPos ]->m_x;
					m_CurPos++;
					continue;
				}
				break;
			}

			//��-- ���݂̋�Ԉʒu���m�� --��//
			m_pCur = m_ppFirst[ m_Offset + m_CurPos ];

			//���̓_�ɑ΂���W�����������Z�o
			return m_cx / m_pCur->m_x;
		}

	};
	typedef typename CInterpolating< VAL , X >::CIterator iterator;

protected:
	//�i�[��
	size_t m_Size;

	//��Ԓl�|�C���^�z��(��[)
	TRI **m_ppTrio;

	//�|�C���^�z��̍Ō�[
	TRI **m_pLast;

public:
	//�R���X�g���N�^
	CInterpolating()
	{
		//�i�[��
		m_Size = 0;

		//��Ԓl�|�C���^�z��(��[) //CTrio�|�C���^�i�[���m��
		m_ppTrio = ( TRI** )malloc( sizeof( TRI* ) );

		//��Ԓl�|�C���^�z��
		*m_ppTrio = INTERPOLATING_ENDPTR;

		//�|�C���^�z��̍Ō�[
		m_pLast  = m_ppTrio;
	}

	//�f�X�g���N�^
	virtual ~CInterpolating()
	{
		//�J��
		clear();
		free( m_ppTrio );
	}

	//�o�^
	void push_back( VAL& val , X& x , W weight = 1.0 )
	{
		//�g���I�𐶐�
		TRI *p = new TRI( val , x , weight );

		//�z���1�g��
		AddAryMemory( 1 );

		//�ǉ������z�񕔕��ɒl���i�[(�|�C���^�̏㏑��)
		Rewrite( m_pLast , p );
	}

	//�i�[���擾
	size_t size()
	{
		return m_Size;
	}

	//�J�n�C�e���[�^�擾
	iterator begin()
	{
		return iterator( m_ppTrio , this );
	}

	//�I�[�C�e���[�^���擾
	iterator end()
	{
		return iterator( NULL , this );
	}

protected: 
	//�z��̊g��
	void AddAryMemory( size_t Cont )
	{
		if( Cont > 0 ){
			//�w��̑傫���Ƀ|�C���^�z����g��
			m_ppTrio = ( TRI** )realloc( m_ppTrio , sizeof( TRI* ) * ( m_Size + Cont ) );

			//�g�����������̒l��������
			for( size_t i = 0 ; i < Cont ; i++ ){
				m_ppTrio[ m_Size + i ] = INTERPOLATING_ENDPTR;
			}

			//�T�C�Y��ύX
			m_Size += Cont;

			//�|�C���^�̐ݒ��ύX
			m_pLast = m_ppTrio + m_Size - 1;
		}
	}

	//�|�C���^�̏㏑��
	void Rewrite( TRI** pPlace , TRI* val )
	{
		//�㏑�������̃|�C���^����폜
		if( ( *pPlace ) != INTERPOLATING_ENDPTR ){
			delete *pPlace;
		}

		*pPlace = val;
	}

	//�J��
	void clear()
	{
		//�m�ۂ�����������S�ăN���A����
		for( size_t i = 0 ; i < _Size ; i++){
			delete m_ppTrio[ i ];
		}
		m_Size = 0;
		m_ppTrio = ( TRI** )realloc( m_ppTrio , sizeof( TRI* ) );
		*m_ppTrio = INTERPOLATING_ENDPTR;
		m_pLast  = m_ppTrio;
	}

	// �ŏ��̃|�C���^�ʒu���擾
	TRI** GetFirstPtr()
	{
		return m_ppTrio;
	}
};