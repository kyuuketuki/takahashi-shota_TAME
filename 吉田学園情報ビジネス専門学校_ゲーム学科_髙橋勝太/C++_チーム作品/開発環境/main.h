//=============================================================================
//
// ���C������ [main.h]
// Author : takahashi shota
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_
#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �w�b�_�t�@�C���̃C���N���[�h
//=============================================================================
#define DIRECTINPUT_VERSION (0x0800)
#include <windows.h>
#include "d3dx9.h"						//�`�揈���ɕK�v
#include "dinput.h"						//���͏����ɕK�v
#include "xaudio2.h"					//�T�E���h�Đ��ɕK�v
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//=============================================================================
// ���C�u�����t�@�C���̃����N
//=============================================================================
#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"d3dx9.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"winmm.lib")

//=============================================================================
// �}�N����`
//=============================================================================
#define ID_BUTTON000	(101)				//�{�^����ID
#define ID_EDIT000		(111)				//�G�f�B�b�g��ID
#define TIMER_INTERVAL	(1000/60)			//FPS�\���Ɏg�p
#define SCREEN_WIDTH	(1920)				//��ʂ̕�
#define SCREEN_HEIGHT	(1080)				//��ʂ̍���
#define MAX_POLYGON		(300)				//�`��ł��鐔
#define MAX_LIFE		(5)					//�v���C���[�̃��C�t��

//=============================================================================
// �֐���`
//=============================================================================
int GetFPS(void);

//�O���錾
class CRenderer;
class CManager;

//�v���g�^�C�v�錾
CManager *GetManager(void);

#endif