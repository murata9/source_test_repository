///@file モデルクラス 0612村田昂平
#include "my3dlib.h"
#include "basic3dtex.h"

MODEL MODEL::sModel[MAX_MODEL];//モデル

MODEL::~MODEL()
{
	//モデルの解放
	if (this->used == TRUE)
	{
		if (this->pmaterials != NULL)
		{
			delete[] this->pmaterials;
		}
		if (this->pPTEX != NULL)
		{
			for (DWORD j = 0; j < this->nummaterials; j++)
			{
				if (this->pPTEX[j] != NULL)//テクスチャがないときように追加
				{
					FreeTex(this->pPTEX[j]);
				}
			}
		}
		delete[] this->pPTEX;
		if (this->pmesh != NULL)
		{
			this->pmesh->Release();
		}
	}
}

/**
 *	@brief	Xファイルを読み込む関数
 *	@param	folderPath	フォルダパス	例:"model/"
 *	@param	filename	ファイル名		例:"jiki.x"
 *	@return	-1:失敗	それ以外:モデル配列のインデックス
 *	@author	村田昂平
 *	@date	初期から
 */
MODEL* MODEL::ReturnModel(LPCTSTR folderPath, LPCTSTR filename)
{
	MODEL* pModel = NULL;
	for (int i = 0; i < MAX_MODEL; i++)
	{
		if (sModel[i].used == 0)
		{
			pModel = &sModel[i];
			break;
		}
	}
	if (pModel == NULL) return NULL;
	LPD3DXBUFFER pD3DXMtrlBuffer;	//一時記憶用バッファ
	char loadXfilename[256];
	strcpy_s(loadXfilename, folderPath);
	strcat_s(loadXfilename, filename);
	//Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(loadXfilename, D3DXMESH_SYSTEMMEM,
		g_pD3DDevice, NULL,
		&pD3DXMtrlBuffer, NULL, &pModel->nummaterials,
		&pModel->pmesh)))
	{
		char buf[256];
		sprintf_s(buf, "Xファイルが見つかりません ファイル名:%s", loadXfilename);
		MessageBox(NULL, buf, _T("3D Lib"), MB_OK);
		return NULL;
	}

	//マテリアルとテクスチャ記録用配列の確保
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	int num = pModel->nummaterials;
	pModel->pmaterials = new D3DMATERIAL9[num];
	if (pModel->pmaterials == NULL) return NULL;
	pModel->pPTEX = new TEX*[num];
	if (pModel->pPTEX == NULL) return NULL;

	for (int i = 0; i < num; i++)
	{
		//マテリアルのコピー
		pModel->pmaterials[i] = d3dxMaterials[i].MatD3D;
		//アンビエント色の設定
		pModel->pmaterials[i].Ambient = pModel->pmaterials[i].Diffuse;
		//テクスチャの読み込み
		pModel->pPTEX[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlenA(d3dxMaterials[i].pTextureFilename) > 0)
		{
			//TEX型に変更
			pModel->pPTEX[i] = LoadTex(folderPath, d3dxMaterials[i].pTextureFilename);
			if (pModel->pPTEX[i] == NULL || pModel->pPTEX[i]->pTex == NULL)
			{
				MessageBox(NULL, _T("テクスチャが見つかりません"), _T("3D Lib"), MB_OK);
				return NULL;
			}
		}
	}
	pD3DXMtrlBuffer->Release();
	pModel->used = TRUE;

	return pModel;
}

/**
 *	@brief	アニメーションのないモデル描画関数
 *	@param	model	描画するモデルポインタ
 *	@author	村田昂平
 *	@date	2014/12/16
 *	@note	モデル型ポインタで描画できるように改造 (オーバーロード)
 */
void MODEL::RenderModel()
{
	if (this->used == FALSE)
	{
		MessageBox(NULL, _T("モデルレンダーエラー モデルがありません"), _T("3dlib.cpp"), MB_OK);
		return;
	}
	for (DWORD i = 0; i < this->nummaterials; i++)
	{
		g_pD3DDevice->SetMaterial(&this->pmaterials[i]);
		if (this->pPTEX[i] != NULL)
		{
			g_pD3DDevice->SetTexture(0, this->pPTEX[i]->pTex);
		}
		else
		{
			g_pD3DDevice->SetTexture(0, NULL);
		}
		this->pmesh->DrawSubset(i);
	}
}
