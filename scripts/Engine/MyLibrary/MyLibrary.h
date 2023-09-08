#pragma once
#include "DxLib.h"
#include "../nlohmann/json.hpp"
#include <sstream>
#include <string>
#include <memory>
#include <vector>
#include <fstream>

using json = nlohmann::json;

namespace ML
{
	constexpr float PI = 3.141592f;
	enum VALUE { ZERO, ONE, TWO, THREE, FOUR, FIVE };

	class Color;
	class Size2;
	class Size3;
	class Vec2;
	class Vec3;
	class Mat4x4;
	class Box2D;
	class Rect2;
	class Transform;
	class Image;
	class PartImage;
	class Camera;
	class Font;
	class InKeyBoard;
	class InMouse;

	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//�V�K�N���X�����N���X
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	class GameData {
	public:
		virtual std::string DebugPrint() const = 0;
		virtual std::string DebugPrintLine() const;

	};


	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//�F�N���X
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	class Color : virtual public GameData {
	public:
		float a;		//���n
		float r;		//��
		float g;		//��
		float b;		//��

		//�f�t�H���g�R���X�g���N�^
		Color();

		//�R���X�g���N�^
		Color(float a_, float r_, float g_, float b_);

		//���Z�q��`
		friend std::istream& operator>>(std::istream& is_, Color& col_);

		//json�t�@�C���ւ̓ǂݏ���
		friend void to_json(json& j_, const Color& color_);
		friend void from_json(const json& j_, Color& color_);

		//DxLib�ɂ���COLOR_F�ւ̃L���X�g��`
		operator DxLib::COLOR_F() const;

		//�F���ԋp
		std::string DebugPrint() const;
	};
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//2�����T�C�Y�N���X
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	class Size2 : virtual public GameData {
	public:
		float w;		//����
		float h;		//�c��

		//�f�t�H���g�R���X�g���N�^
		Size2();

		//�R���X�g���N�^
		Size2(float w_, float h_);

		//���Z�q��`
		friend std::istream& operator>>(std::istream& is_, Size2& size_);
		
		//json�t�@�C���ւ̓ǂݏ���
		friend void to_json(json& j_, const Size2& size_);
		friend void from_json(const json& j_, Size2& size_);

		//�f�o�b�O���
		std::string DebugPrint() const;
	};

	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//3�����T�C�Y�N���X
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	class Size3 : virtual public Size2 {
	public:
		float d;		//���s�idepth�j

		//�f�t�H���g�R���X�g���N�^
		Size3();

		//�R���X�g���N�^
		Size3(float w_, float h_, float d_);

		//���Z�q��`
		friend std::istream& operator>>(std::istream& is_, Size3& size_);

		//json�t�@�C���ւ̓ǂݏ���
		friend void to_json(json& j_, const Size3& size_);
		friend void from_json(const json& j_, Size3& size_);

		//�f�o�b�O���
		std::string DebugPrint() const;
	};

	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//2�����x�N�g���N���X
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	class Vec2 : virtual public GameData {
	public:
		float x;
		float y;

		//�f�t�H���g�R���X�g���N�^
		Vec2();

		//�R���X�g���N�^
		Vec2(float x_, float y_);

		//�f�o�b�O���
		std::string DebugPrint() const;

		//���Z�q��`
		friend Vec2 operator+(const Vec2& vecL_, const Vec2& vecR_);
		friend Vec2 operator-(const Vec2& vecL_, const Vec2& vecR_);
		friend Vec2 operator*(const Vec2& vecL_, const Vec2& vecR_);
		friend Vec2 operator*(const Vec2& vecL_, float value_);
		Vec2& operator+= (const Vec2& vec_);
		Vec2& operator-=(const Vec2& vec_);
		friend std::istream& operator>>(std::istream& is_, Size3& size_);

		//json�t�@�C���ւ̓ǂݏ���
		friend void to_json(json& j_, const Vec2& vec_);
		friend void from_json(const json& j_, Vec2& vec_);
	};

	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//3�����x�N�g���N���X
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	class Vec3 : virtual public Vec2 {
	public:
		float z;

		//�f�t�H���g�R���X�g���N�^
		Vec3();

		//�R���X�g���N�^
		Vec3(float x_, float y_, float z_);

		//�f�o�b�O���
		std::string DebugPrint() const;

		//�傫��
		float Length() const;

		//���K��
		Vec3 Norm() const;

		//����
		friend float VDot(const Vec3& vecL_, const Vec3& vecR_);

		//�O��
		friend Vec3 VCross(const Vec3& vecL_, const Vec3& vecR_);
		
		//���Z�q��`
		Vec3 operator-();
		friend Vec3 operator+(const Vec3& vecL_, const Vec3& vecR_);
		friend Vec3 operator-(const Vec3& vecL_, const Vec3& vecR_);
		friend Vec3 operator*(const Vec3& vec_, float scale_);
		Vec3& operator+= (const Vec3& vec_);
		Vec3& operator-=(const Vec3& vec_);
		Vec3& operator*=(const Mat4x4& mat_);
		Vec3& operator*=(float value_);
		bool  operator==(float value_);
		bool  operator!=(float value_);
		friend std::istream& operator>>(std::istream& is_, Vec3& vec_);
		friend std::ostream& operator<<(std::ostream& os_, const Vec3& vec_);

		//json�t�@�C���ւ̓ǂݏ���
		friend void to_json(json& j_, const Vec3& vec_);
		friend void from_json(const json& j_, Vec3& vec_);

		//DxLib�ɂ���VECTOR�ւ̃L���X�g��`
		operator DxLib::VECTOR() const;
	};
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//���C�N���X
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[

	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//�s��N���X
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	class Mat4x4 : virtual public GameData, public DxLib::MATRIX {
	public:
		Mat4x4(bool identity_ = false);
		Mat4x4(const MATRIX& v_);
		Mat4x4  operator= (const MATRIX& v_);
		//�P�ʍs��
		void  Identity();
		//���s�ړ��s��
		void  Translation(const Vec3& vec_);
		//X����]�s��
		void RotationX(float degree_);
		//Y����]�s��
		void RotationY(float degree_);
		//Z����]�s��
		void RotationZ(float degree_);
		//��]�s��E���[���s�b�`���[�iXYZ�j�̏�
		void RotationXYZ(const Vec3& ang_);
		//�C�ӎ���]�s��
		void RotationFree(const Vec3& axis_, float degree_);
		//�T�C�Y�ύX
		void Scale(float sizeX_, float sizeY_, float sizeZ_);

		//json�t�@�C���ւ̓ǂݏ���
		friend void to_json(json& j_, const Mat4x4& vec_);
		friend void from_json(const json& j_, Mat4x4& vec_);

		//�f�o�b�O���
		std::string DebugPrint() const;

		//���Z�q��`
		friend Mat4x4 operator*(const Mat4x4& matL_, const Mat4x4& matR_);
		friend Vec3 operator*(const Vec3& vecL_, const Mat4x4& matR_);
		Mat4x4& operator*=(const Mat4x4& mat_);
		friend std::istream& operator>>(std::istream& is_, Mat4x4& mat_);

		//json�t�@�C���ւ̓ǂݏ���
		friend void to_json(json& j_, const Mat4x4& mat_);
		friend void from_json(const json& j_, Mat4x4& mat_);
	};

	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//2�����{�b�N�X�N���X
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	class Box2D : virtual public Vec2, virtual public Size2 {
	public:
		//�f�t�H���g�R���X�g���N�^
		Box2D();

		//�R���X�g���N�^
		Box2D(float x_, float y_, float w_, float h_, bool center_ = false);
		Box2D(float x_, float y_, const Size2& size_, bool center_ = false);
		Box2D(const Vec2& vec_, float w_, float h_, bool center_ = false);
		Box2D(const Vec2& vec_, const Size2& size_, bool center_ = false);

		//�������ړ�������
		void Offset(float x_, float y_);
		void Offset(const Vec2& vec_);

		//���S�����_�Ƃ��Ĉ������ړ�������
		void OffsetCenterBase(float x_, float y_);
		void OffsetCenterBase(const Vec2& v_);

		//�ړ��������N���[���𐶐�
		Box2D OffsetCopy(float x_, float y_);
		Box2D OffsetCopy(const Vec2& vec_);

		//���S�����_�Ƃ��Ĉ������ړ��������N���[���𐶐�
		Box2D OffsetCenterBaseCopy(float x_, float y_);
		Box2D OffsetCenterBaseCopy(const Vec2& vec_);

		//�����̍��W�Ɉړ�������
		void Set(float x_, float y_);
		void Set(const Vec2& vec_);

		//�����̍��W�ֈړ��������N���[���𐶐�
		Box2D SetCopy(float x_, float y_);
		Box2D SetCopy(const Vec2& vec_);

		//���S�����_�Ƃ��Ĉړ��������N���[���𐶐�
		Box2D SetCenterBaseCopy(float x_, float y_);
		Box2D SetCenterBaseCopy(const Vec2& vec_);

		//���S����_�ɍ��W�w��
		void SetCenterBase(float x_, float y_);
		void SetCenterBase(const Vec2& vec_);

		//�T�C�Y�Đݒ�
		void Resize(float w_, float h_);
		void Resize(const Size2& size_);

		//�����蔻��
		bool Hit(const Box2D& you_);

		//���Z�q��`
		friend std::istream& operator>>(std::istream& is_, Box2D& box_);

		//json�t�@�C���ւ̓ǂݏ���
		friend void to_json(json& j_, const Box2D& box_);
		friend void from_json(const json& j_, Box2D& box_);
		
		//�f�o�b�O���
		std::string DebugPrint() const;

	};

	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//3�����{�b�N�X�N���X
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	class Box3D : virtual public Vec3, virtual public Size3, virtual public Box2D{
	public:
		//�f�t�H���g�R���X�g���N�^
		Box3D();

		//�R���X�g���N�^
		Box3D(float x_, float y_, float z_, float w_, float h_, float d_, bool center_ = false);
		Box3D(float x_, float y_, float z_, const Size3& size_, bool center_ = false);
		Box3D(const Vec3& vec_, float w_, float h_, float d_, bool center_ = false);
		Box3D(const Vec3& vec_, const Size3& size_, bool center_ = false);

		//�������ړ�������
		void Offset(float x_, float y_, float z_);
		void Offset(const Vec3& vec_);

		//���S�����_�Ƃ��Ĉ������ړ�������
		void OffsetCenterBase(float x_, float y_, float z_);
		void OffsetCenterBase(const Vec3& vec_);

		//�ړ��������N���[���𐶐�
		Box3D OffsetCopy(float x_, float y_, float z_);
		Box3D OffsetCopy(const Vec3& vec_);

		//���S�����_�Ƃ��Ĉ������ړ��������N���[���𐶐�
		Box3D OffsetCenterBaseCopy(float x_, float y_, float z_);
		Box3D OffsetCenterBaseCopy(const Vec3& vec_);

		//�����̍��W�Ɉړ�������
		void Set(float x_, float y_, float z_);
		void Set(const Vec3& vec_);

		//�����̍��W�ֈړ��������N���[���𐶐�
		Box3D SetCopy(float x_, float y_, float z_);
		Box3D SetCopy(const Vec3& vec_);

		//���S�����_�Ƃ��Ĉړ��������N���[���𐶐�
		Box3D SetCenterBaseCopy(float x_, float y_, float z_);
		Box3D SetCenterBaseCopy(const Vec3& vec_);

		//���S����_�ɍ��W�w��
		void SetCenterBase(float x_, float y_, float z_);
		void SetCenterBase(const Vec3& vec_);

		//�T�C�Y�Đݒ�
		void Resize(float w_, float h_, float d_);
		void Resize(const Size3& size_);

		//�����蔻��
		bool Hit(const Box3D& you_);

		//���Z�q��`
		friend std::istream& operator>>(std::istream& is_, Box3D& box_);

		//json�t�@�C���ւ̓ǂݏ���
		friend void to_json(json& j_, const Box3D& box_);
		friend void from_json(const json& j_, Box3D& box_);

		//�f�o�b�O���
		std::string DebugPrint() const;

	};
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//�p�x�t�������̃N���X�i���S�����_�j
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	class Obb3 :virtual public Vec3, virtual public Size3 {
	private:
		Vec3 dirX;		//�����x�N�g��X
		Vec3 dirY;		//�����x�N�g��Y
		Vec3 dirZ;		//�����x�N�g��Z

	public:

		//�f�t�H���g�R���X�g���N�^
		Obb3();

		//�R���X�g���N�^
		Obb3(const Vec3& pos_, const Size3& size_);

		//�����x�N�g��Y���X�V���A����ɍ��킹��X,Z���X�V����
		//�i�����Ő��K�����s����j
		void SetDirectionY(const Vec3& vec_);

		//�ړ�
		void Offset(const Vec3& pos_);
		//�R�s�[
		Obb3 OffsetCopy(const Vec3& pos_) const;

		//���W�w��
		void SetPos(const Vec3& pos_);
		//�R�s�[
		Obb3 SetPosCopy(const Vec3& pos_) const;

		//�T�C�Y�Đݒ�
		void Resize(const Size3& size_);

		//�Q�b�^
		Vec3 get_dirX() const { return dirX; }
		Vec3 get_dirY() const { return dirY; }
		Vec3 get_dirZ() const { return dirZ; }
		Vec3 get_pos() const { return Vec3(x, y, z); }
		Size3 get_size() const { return Size3(w, h, d); }

		//���Z�q��`
		friend std::istream& operator>>(std::istream& is_, Obb3& obb_);

		//json�t�@�C���ւ̓ǂݏ���
		friend void to_json(json& j_, const Obb3& box_);
		friend void from_json(const json& j_, Obb3& box_);

		//�f�o�b�O���
		std::string DebugPrint() const;
	};
	//�����蔻��
	bool CheckHit(const Obb3& obb1_, const Obb3& obb2_);

	// �������ɓ��e���ꂽ���������瓊�e���������Z�o
	float LenSegOnSeparateAxis(const Vec3& sep, const Vec3& e1, const Vec3& e2, const Vec3& e3 = Vec3());

	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//2������`�N���X
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	class Rect2 :virtual public GameData {
	public:
		float l;	//��(x)	left
		float r;	//�E(x)	right
		float t;	//��(y)	top
		float b;	//��(y)	bottom

		//�f�t�H���g�R���X�g���N�^
		Rect2();

		//�R���X�g���N�^
		Rect2(float l_, float r_, float t_, float b_);
		Rect2(const ML::Box2D& box_);

		//�{�b�N�X����ϊ�
		void BoxToRect(const ML::Box2D& box_);

		//���Z�q��`
		friend std::istream& operator>>(std::istream& is_, Rect2& rect_);

		//json�t�@�C���ւ̓ǂݏ���
		friend void to_json(json& j_, const Rect2& box_);
		friend void from_json(const json& j_, Rect2& box_);

		//�f�o�b�O���
		std::string DebugPrint() const;
	};

	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//3���������̃N���X
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	class Rect3 :virtual public Rect2 {
	public:
		float n;	//��O(z)	near
		float f;	//���iz)	far

		//�f�t�H���g�R���X�g���N�^
		Rect3();

		//�R���X�g���N�^
		Rect3(float l_, float t_, float r_, float b_, float n, float f_);
		Rect3(const ML::Box3D& box_);

		//�{�b�N�X����ϊ�
		void BoxToRect(const ML::Box3D& box_);

		//���Z�q��`
		friend std::istream& operator>>(std::istream& is_, Rect3& rect_);

		//json�t�@�C���ւ̓ǂݏ���
		friend void to_json(json& j_, const Rect3& box_);
		friend void from_json(const json& j_, Rect3& box_);

		//�f�o�b�O���
		std::string DebugPrint() const;
	};
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//�ό`�N���X
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	class Transform :virtual public GameData {
	public:
		float rotateDeg;			//�p�x�i�x���@�j
		Vec2  rotateCenterPos;			//��]�̒��S
		float scaleX;			//����
		float scaleY;			//�c��
		bool  flipX;			//X�������]
		bool  flipY;			//Y�������]
		
		//�f�t�H���g�R���X�g���N�^
		Transform(float rotDeg_ = 0.0f, ML::Vec2 rotCt_ = ML::Vec2(), float scX_ = 1.0f, float scY_ = 1.0f, bool flipX_ = false, bool flipY_ = false);

		//���Z�q��`
		friend std::istream& operator>>(std::istream& is_, Transform& trs_);

		//json�t�@�C���ւ̓ǂݏ���
		friend void to_json(json& j_, const Transform& trs_);
		friend void from_json(const json& j_, Transform& trs_);

		//�f�o�b�O���
		std::string DebugPrint() const;
	};
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//�摜�N���X
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	class Image : virtual public GameData {
	private:
		std::string filePath;		//�t�@�C���ւ̃p�X
		int handle;			//�摜���[�h���ɓn����郆�j�[�N�Ȓl
		Size2 size;			//�摜�T�C�Y
	protected:
		//�R���X�g���N�^
		Image(char* filePath_, const Size2& size_);
		Image(const char* filePath, const Size2& size_);
		Image(std::string filePath_, const Size2& size_);
	public:
		using SP = std::shared_ptr<Image>;

		//����
		static Image::SP Create(char* filePath_, const Size2& size_ = Size2());
		static Image::SP Create(const char* filePath_, const Size2& size_ = Size2());
		static Image::SP Create(std::string filePath_, const Size2& size_ = Size2());

		//�Q�b�^
		Size2 get_size() { return size; }

		//���S���W���󂯎��
		Vec2  CenterPos();

		//�`��
		void Draw(int x_, int y_, bool TransFlag = true, float alpha_ = 1.0f);
		void Draw(Vec2 pos_, bool TransFlag = true, float alpha_ = 1.0f);
		//���S�����_�ɂ��ĕ`��
		void DrawCt(int x_, int y_, bool TransFlag = true, float alpha_ = 1.0f);
		void DrawCt(Vec2 pos_, bool TransFlag = true, float alpha_ = 1.0f);
		//���S�����_�ɂ��ĕό`�`��
		void DrawTransform(Vec2 posCt_, Transform trs_ = Transform(), bool TransFlag = true, float alpha = 1.0f);

		//�f�o�b�O���
		std::string DebugPrint() const;
	};

	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//�����摜�N���X
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	class PartImage : virtual public GameData {
	private:
		std::string filePath;		//�t�@�C���ւ̃p�X
		int* handleArray;		//�摜���[�h���ɓn����郆�j�[�N�Ȓl�̔z��
		int AllNum;			//�����摜����
		int xCount;			//���̕�����
		int yCount;			//�c�̕�����
		int xSize;			//���������摜�̉���
		int ySize;			//���������摜�̏c��
	public:
		using SP = std::shared_ptr<PartImage>;


		//����
		static PartImage::SP Create(char* filePath_, int AllNum_, int xCnt_, int yCnt_, int xSize_, int ySize_);
		static PartImage::SP Create(const char* filePath_, int AllNum_, int xCnt_, int yCnt_, int xSize_, int ySize_);
		static PartImage::SP Create(std::string filePath_, int AllNum_, int xCnt_, int yCnt_, int xSize_, int ySize_);



		//�R���X�g���N�^
		PartImage(const std::string& filePath_, int AllNum_, int xCnt_, int yCnt_, int xSize_, int ySize_);

		//�f�X�g���N�^
		~PartImage();

		//�Q�b�^
		int get_AllNum() { return AllNum; }
		int get_xCount() { return xCount; }
		int get_yCount() { return yCount; }
		int get_xSize() { return xSize; }
		int get_ySize() { return ySize; }


		//�`��
		void Draw(int x_, int y_, int imageNum_, bool TransFlag = true, float alpha_ = 1.0f);
		void Draw(Vec2 pos_, int imageNum_, bool TransFlag = true, float alpha_ = 1.0f);


		//�f�o�b�O���
		std::string DebugPrint() const;
	};


	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//�t�H���g�N���X
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	class Font : virtual public GameData {
	public:
		using SP = std::shared_ptr<Font>;
	protected:
		int handle;		//�t�H���g�n���h��
		char* fontName;	//�t�H���g��
		int size;		//�T�C�Y
		int thick;		//����
		int fontType;	//�t�H���g�̃^�C�v
	public:
		//����
		static Font::SP Create();
		static Font::SP Create(const char* fontName_, int size_, int thick_ = -1, int fontType_ = -1);

		//�f�t�H���g�R���X�g���N�^
		Font();

		//�R���X�g���N�^
		Font(const char* fontName_, int size_, int thick_, int fontType_ = -1);

		//�Q�b�^
		int get_size() const { return size; }

		void Draw(int x_, int y_, const char* str_, const Color& color_ = Color(1, 1, 1, 1));
		void Draw(Vec2 pos_, const char* str_, const Color& color_ = Color(1, 1, 1, 1));

		//�f�o�b�O���
		std::string DebugPrint() const;
	};

	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//���b�V���N���X
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	class Model3D :virtual public GameData {
	private:
		//�R���X�g���N�^
		Model3D(const std::string& filePath_);
	protected:
		std::string filePath;		//�t�@�C���p�X
		int handle;			//���b�V���ւ̃n���h��
		ML::Color diffuse;			//�f�B�t���[�Y�i�g�U���ˁj
		ML::Color specler;			//�X�y�L�����i���ʔ��ˁj
		ML::Color emissive;		//�G�~�b�V�u�i�����j
		ML::Color ambient;		//�A���r�G���g�i�����j

		Model3D() {};		//�f�t�H���g�R���X�g���N�^


	public:
		//�f�X�g���N�^
		~Model3D();
		using SP = std::shared_ptr<Model3D>;

		//����
		static SP Create(char* filePath_);
		static SP Create(const char* filePath_);
		static SP Create(const std::string& filePath_);

		//�Q�b�^
		std::string get_filePath() { return filePath; }
		int get_handle() { return handle; }

		//�}�e���A���ݒ�
		void SetDiffuse(ML::Color& dif_);
		void SetSpecler(ML::Color& spec_); 
		void SetEmissive(ML::Color& emi_);
		void SetAmbient(ML::Color& amb_);

		//�`��
		virtual void Draw(const Mat4x4& matWorld_);
		virtual void DrawStd(const Vec3& pos_, const Vec3& angle_ = Vec3(0, 0, 0), const Vec3& scale_ = Vec3(1, 1, 1));

	public:
		//�f�o�b�O���
		std::string DebugPrint() const;
	};
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//���b�V�������p�N���X
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	class Model3DClone : virtual protected Model3D {
	private:
		std::weak_ptr<Model3D> wp_base;
	public:
		//�R���X�g���N�^
		Model3DClone(Model3D::SP baseModel_);
		//�f�X�g���N�^
		~Model3DClone();
		using SP = std::shared_ptr<Model3DClone>;

		//����
		static Model3DClone::SP Create(Model3D::SP baseModel_);

		//�}�e���A���ݒ�
		using Model3D::SetDiffuse;
		using Model3D::SetSpecler;
		using Model3D::SetEmissive;
		using Model3D::SetAmbient;

		//�`��
		void Draw(const Mat4x4& matWorld_);
		void DrawStd(const Vec3& pos_, const Vec3& angle_ = Vec3(0, 0, 0), const Vec3& scale_ = Vec3(1, 1, 1));

		//�f�o�b�O���
		std::string DebugPrint() const;
	};
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//���̓N���X
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	class Input : virtual public GameData {
	public:
		virtual void UpDate() = 0;			//���͏��X�V����
		virtual bool Down(int key_) = 0;	//���͊J�n���̃t���[���Ȃ�true
		virtual bool On(int key_) = 0;		//���͒��Ȃ�true
		virtual bool Up(int key_) = 0;		//���͏I�����̃t���[���Ȃ�true
		virtual bool Off(int key_) = 0;		//���͖����Ȃ�true
	};
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//�L�[�{�[�h�N���X
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	class InKeyBoard : virtual public Input {
	public:
		static const int keysSize = 256;

		char preInp[keysSize];		//1�t���[���O�̓��͏��
		char inp[keysSize];			//���͏��

		//�f�t�H���g�R���X�g���N�^
		InKeyBoard();

		void UpDate()	override;		//���͏��X�V����
		bool Down(int key_) override;	//���͊J�n���̃t���[���Ȃ�true
		bool On(int key_) override;		//���͒��Ȃ�true
		bool Up(int key_) override;		//���͏I�����̃t���[���Ȃ�true
		bool Off(int key_) override;		//���͖����Ȃ�true

		//�f�o�b�O���
		std::string DebugPrint() const;
	};
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//�}�E�X�N���X
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	class InMouse : virtual public Input {
	private:
		static const int buttonsSize = 3;
		enum MI{LEFT, RIGHT, MIDDLE};
		bool preInp[buttonsSize];		//1�t���[���O�̓��͏��
		bool inp[buttonsSize];			//���͏��

		int wheelRotation;				//�z�C�[����]��
		static int  Convert(int macro_);		//�}�E�X�{�^���ԍ����}�N������N���X���̐��l�ɕϊ�
	public:

		//�f�t�H���g�R���X�g���N�^
		InMouse();

		//�ʒu���擾
		Vec2 getPos() const;

		//���͏��֘A
		void UpDate()	override;		//���͏��X�V����
		bool Down(int button_) override;	//���͊J�n���̃t���[���Ȃ�true
		bool On(int button_) override;		//���͒��Ȃ�true
		bool Up(int button_) override;		//���͏I�����̃t���[���Ȃ�true
		bool Off(int button_) override;		//���͖����Ȃ�true

		//�z�C�[���̉�]�ʁi������Ȃ�v���X�A���������Ȃ�}�C�i�X�j
		int GetWheelRotVol();

		//���C���擾����
		ML::Vec3 GetRayNormOfScreen(std::shared_ptr<Camera> cam_, int screenCenterY_) const;

		//�f�o�b�O���
		std::string DebugPrint() const;
	};
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//�J������{�`�N���X
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	class Camera
	{
	protected:
		Camera(
			const ML::Vec3& tg_,	//	��ʑ̂̈ʒu
			const ML::Vec3& pos_,	//	�J�����̈ʒu
			const ML::Vec3& up_,	//	�J�����̏�����������x�N�g���i��̂x�{�Œ�j
			float				fov_,	//	����p
			float				np_,	//	�O�N���b�v���ʁi������O�͉f��Ȃ��j
			float				fp_,	//	��N���b�v���ʁi��������͉f��Ȃ��j
			float				asp_,	//	�A�X�y�N�g��i��ʂ̔䗦�ɍ��킹��@�����c�j	
			ML::Box2D			screen_);	//��ʂɕ`�悷��͈�		
	public:
		//	�r���[���i�����֘A�j
		ML::Vec3 target;			//	��ʑ̂̈ʒu
		ML::Vec3 pos;			//	�J�����̈ʒu
		ML::Vec3 up;				//	�J�����̏�����������x�N�g���i��̂x�{�Œ�j
		//	�ˉe���i����͈͊֘A�j
		float fov;					//	����p
		float nearPlane;			//	�O�N���b�v���ʁi������O�͉f��Ȃ��j
		float forePlane;			//	��N���b�v���ʁi��������͉f��Ȃ��j
		float aspect;				//	�A�X�y�N�g��i��ʂ̔䗦�ɍ��킹��@�����c�j
		ML::Box2D screenArea;		//�@��ʂɕ`�悷��͈�
		//	�s����
		ML::Mat4x4  matView, matProj;

		using SP = std::shared_ptr<Camera>;
		//�@����
		//	�J�����𐶐�����
		static SP Create(
			const ML::Vec3& tg_,	//	��ʑ̂̈ʒu
			const ML::Vec3& pos_,	//	�J�����̈ʒu
			const ML::Vec3& up_,	//	�J�����̏�����������x�N�g���i��̂x�{�Œ�j
			float				fov_,	//	����p
			float				np_,	//	�O�N���b�v���ʁi������O�͉f��Ȃ��j
			float				fp_,	//	��N���b�v���ʁi��������͉f��Ȃ��j
			float				asp_,	//	�A�X�y�N�g��i��ʂ̔䗦�ɍ��킹��@�����c�j	
			ML::Box2D			screen_);	//��ʂɕ`�悷��͈�

		~Camera();
		//	�J�����̐ݒ�
		void update();

		//�`�悷��͈͂̒��S���W��Ԃ�
		ML::Vec2 getViewScreenCenter();
	};



	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//�֐���`
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//Color��DX���C�u�����ɑΉ�������
	unsigned int GetColor(const Color& color_);
	DxLib::COLOR_U8 GetColorU8(const Color& color_);
	//�x���@���烉�W�A���l�ɕϊ�
	float ToRadian(float degree_);
	//���W�A���l����x���@�ɕϊ�
	float ToDegree(float radian_);
	//Dx���C�u�����ŗp�ӂ���Ă����`�`��𗘗p����
	bool DrawBox(const Box2D& box_, const Color& color_, bool fillFlag_ = true);
	//DX���C�u�����֐����g�p���ă{�b�N�X��\��������
	void DrawBox3D(const Box3D& box_, const Color& col_ = Color(1, 1, 1, 1));
	void DrawBox3D(const Obb3& box_, const Color& col_ = Color(1, 1, 1, 1));


	//--------------------------------------
	// �e���v���[�g�֐�
	//--------------------------------------
	//�f�[�^�t�@�C������f�[�^��ǂ݂���
	template <class Type>
	void ReadData(std::istream& is_, Type& type_, bool isbin_ = false) {
		//�ʏ�̓ǂݍ���
		if (isbin_ == false) {
			std::string data;
			getline(is_, data);
			//�u>>>�v�܂ł̕����͖�������
			while (data[0] != '>' 
				|| data[1] != '>'
				|| data[2] != '>') {
				data.erase(data.begin());
			}
			//�u>>>�v���폜
			data.erase(data.begin(), (data.begin() + 3));
			//�ǂݍ��񂾃f�[�^���2�����ɑ������
			std::stringstream ss(data);
			ss >> type_;
		}
		//�o�C�i���f�[�^�̓ǂݍ���
		else {
			is_.read((char*)&type_, sizeof(type_));
		}
	}
}