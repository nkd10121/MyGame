#include "Input.h"
#include <DxLib.h>

using namespace std;

Input::Input()
{
	m_commandTable["OK"] = { {InputType::KeyBoard, KEY_INPUT_RETURN},
							{InputType::Pad,PAD_INPUT_A} };

	m_commandTable["CANCEL"] = { {InputType::KeyBoard, KEY_INPUT_ESCAPE},
							{InputType::Pad,PAD_INPUT_B} };

	m_commandTable["pause"] = { {InputType::KeyBoard, KEY_INPUT_ESCAPE},
								{InputType::Pad, PAD_INPUT_R}};

	m_commandTable["dash"] = { {InputType::KeyBoard, KEY_INPUT_SPACE},
								{InputType::Pad, PAD_INPUT_A} };

#ifdef _DEBUG
	m_commandTable["end"] = { {InputType::KeyBoard, KEY_INPUT_R},
							{InputType::Pad, PAD_INPUT_Y} };

	m_commandTable["Goal"] = { {InputType::KeyBoard, KEY_INPUT_G},
							{InputType::Pad, PAD_INPUT_X} };
#endif

	m_commandTable["UP"] = { {InputType::KeyBoard, KEY_INPUT_W},
								{InputType::Pad, PAD_INPUT_UP} };

	m_commandTable["RIGHT"] = { {InputType::KeyBoard, KEY_INPUT_D},
								{InputType::Pad, PAD_INPUT_RIGHT} };

	m_commandTable["DOWN"] = { {InputType::KeyBoard, KEY_INPUT_S},
								{InputType::Pad, PAD_INPUT_DOWN} };

	m_commandTable["LEFT"] = { {InputType::KeyBoard, KEY_INPUT_A},
								{InputType::Pad, PAD_INPUT_LEFT} };
}

void Input::Update()
{

	m_lastInputData = m_inputData;	//���O���͂��R�s�[���Ă���(�������u�Ԃ��擾����p)

	//�n�[�h�E�F�A���̓`�F�b�N
	char keystate[256];
	GetHitKeyStateAll(keystate);//���݂̃L�[�{�[�h���͂��擾
	int padstate = GetJoypadInputState(DX_INPUT_PAD1);//�p�b�h���̎擾

	//�o�^���ꂽ���ƃn�[�h�̏����Ƃ炵���킹�Ȃ���
	//inputData_�̓��e���X�V���Ă���
	for (const auto& cmd : m_commandTable)
	{
		auto& test = cmd.first;

		auto& input = m_inputData[cmd.first];//�R�}���h�̖��O������̓f�[�^�����

		for (const auto& hardIO : cmd.second)
		{
			//�L�[�{�[�h�̃`�F�b�N
			input = false;
			if (hardIO.first == InputType::KeyBoard)
			{
				if (keystate[hardIO.second])
				{
					input = true;
					break;
				}
			}
			else if (hardIO.first == InputType::Pad)
			{
				if (padstate & hardIO.second)
				{
					input = true;
					break;
				}
			}
		}
	}
}

bool Input::IsTriggered(const char* command) const
{
	auto it = m_inputData.find(command);
	if (it == m_inputData.end())	//��񂪂Ȃ����false��Ԃ�
	{
		return false;
	}
	return (m_inputData.at(command) && !m_lastInputData.at(command));
}

bool Input::IsPushed(const char* command) const
{
	auto it = m_inputData.find(command);
	if (it == m_inputData.end())	//��񂪂Ȃ����false��Ԃ�
	{
		return false;
	}
	return (m_inputData.at(command)&& m_lastInputData.at(command));
}
