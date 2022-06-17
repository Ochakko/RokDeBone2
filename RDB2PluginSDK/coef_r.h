#ifndef COEFRH
#define COEFRH

#define	DEG2PAI		0.01745329251994


//JointInfo
enum {
	JI_SERIAL = 0, 
	JI_NOTUSE,
	JI_PARENT,
	JI_CHILD,
	JI_BROTHER,
	JI_SISTER,
	JI_MAX
};

//DispObjInfo
enum {
	DOI_SERIAL = 0,
	DOI_NOTUSE,
	DOI_DISPSWITCH,
	DOI_INVISIBLE,
	DOI_MAX
};

//ShaderType
enum {
	COL_OLD,
	COL_PHONG,
	COL_BLINN,
	COL_SCHLICK,
	COL_MQCLASSIC,
	COL_TOON0,
	COL_TOON1,
	COL_MAX
};

//Shader�̐F���I�[�o�[�t���[�����Ƃ��̏���
enum {
	OVERFLOW_CLAMP,
	OVERFLOW_SCALE,
	OVERFLOW_ORG,
	OVERFLOW_MAX
};

//SetInfElem, GetInfElem
	//SetInfElem�̃y�C���g���[�h
enum {
	PAINT_NOR,//�ʏ�Z�b�g
	PAINT_EXC,//�r���Z�b�g
	PAINT_ADD,//���Z�Z�b�g
	PAINT_SUB,//���Z�Z�b�g
	PAINT_ERA,//����
	PAINT_MAX
};

enum tag_calcmode
{
	CALCMODE_NOSKIN0,//�X�L�j���O����
	CALCMODE_ONESKIN0,//�����Ɗp�x�ŃX�L�j���O
	CALCMODE_ONESKIN1,//�����݂̂ŃX�L�j���O
	CALCMODE_DIRECT0,//���l���ڎw��
	CALCMODE_SYM,//�Ώ̃Z�b�g
	CALCMODE_MAX
};

#endif