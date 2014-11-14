#pragma once 

#include "../../Library/Graphic/graphic_base.h"
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <memory>

#pragma warning ( disable : 4018 ) //signed と unsigned の数値を比較しようとしました。

class CXFile;
typedef std::shared_ptr< CXFile > XFileSP;

//Xファイルクラス
class CXFile : public CGraphicBase{
public:
	//コンストラクタ
	CXFile(){}

	//デストラクタ
	virtual ~CXFile(){}

	//モデル作成
	//引数1:ファイルパス
	static XFileSP Create( const std::string& _path );

	//モデルロード
	//引数1:ファイルパス
	void Load( const std::string& _path );
};
