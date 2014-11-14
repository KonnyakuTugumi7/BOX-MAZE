#pragma once 

#include "../../Library/Graphic/graphic_base.h"
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <memory>

#pragma warning ( disable : 4018 ) //signed �� unsigned �̐��l���r���悤�Ƃ��܂����B

class CXFile;
typedef std::shared_ptr< CXFile > XFileSP;

//X�t�@�C���N���X
class CXFile : public CGraphicBase{
public:
	//�R���X�g���N�^
	CXFile(){}

	//�f�X�g���N�^
	virtual ~CXFile(){}

	//���f���쐬
	//����1:�t�@�C���p�X
	static XFileSP Create( const std::string& _path );

	//���f�����[�h
	//����1:�t�@�C���p�X
	void Load( const std::string& _path );
};
