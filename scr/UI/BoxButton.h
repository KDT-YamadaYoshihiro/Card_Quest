#pragma once
#include <SFML/Graphics.hpp>  
#include <string>  

class BoxButton  
{  
private:  
   sf::RectangleShape m_shape;  
   sf::Font m_font;  
   sf::Text m_text;  

public:  
   // �R���X�g���N�^�F�T�C�Y�A�ʒu�A�t�H���g�A�\��������  
   BoxButton(sf::Vector2f arg_size, sf::Vector2f arg_pos, const sf::Font& arg_font, const std::string& arg_label)  
       : m_font(arg_font),  
         m_text(arg_font, "")  
   {  
       // 1. �{�^���{�̂̐ݒ�i�T�C�Y�A���W�A�F�j  
       m_shape.setSize(arg_size);  
       m_shape.setOrigin(arg_size / 2.0f);  
       m_shape.setPosition(arg_pos);  
       m_shape.setFillColor(sf::Color(200, 200, 200));  

       // --- �e�L�X�g�̐ݒ�i���������΍􍞂݁j ---
       m_text.setString(sf::String::fromUtf8(arg_label.begin(), arg_label.end()));
       m_text.setCharacterSize(Constants::FONT_SIZE_MEDIUM);
       m_text.setFillColor(sf::Color::Black);  

       // 3. �e�L�X�g�`��i���������̌v�Z�j  
       sf::FloatRect textRect = m_text.getLocalBounds();  
       m_text.setOrigin({  
           textRect.position.x + textRect.size.x / 2.0f,  
           textRect.position.y + textRect.size.y / 2.0f  
       });  
       m_text.setPosition(arg_pos);  
   }  

   // �`��  
   void Draw(sf::RenderWindow& window) const  
   {  
       window.draw(m_shape);  
       window.draw(m_text);  
   }  

   // �z�o�[���Ă��邩  
   bool IsHovered(sf::Vector2f mousePos) const  
   {  
       return m_shape.getGlobalBounds().contains(mousePos);  
   }  

   // �N���b�N���ꂽ��  
   bool IsClicked(sf::Vector2f mousePos, bool mousePressed) const  
   {  
       return mousePressed && IsHovered(mousePos);  
   }  

   // �e�L�X�g�̓��e���ォ��ύX�������ꍇ  
   void SetString(const std::string& label)  
   {  
       m_text.setString(label);  
       sf::FloatRect textRect = m_text.getLocalBounds();  
       m_text.setOrigin({ textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f });  
   }  

   //�@�{�^���̐F��ύX�������ꍇ
   void SetColor(const sf::Color& color)  
   {  
       m_shape.setFillColor(color);  
   }

};  

