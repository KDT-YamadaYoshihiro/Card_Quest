#pragma once  
#include <memory>  
#include <vector>  
#include <random>  
#include <algorithm>  
#include "../Card.h"  

class Character;  

// カード使用時
struct CardUseResult
{
    CardData effect;
    int ownerID;
};

// カード管理  
class CardManager 
{  
   // デッキカード  
   std::vector<std::unique_ptr<Card>> m_deck;  

   // 手札カード  
   std::vector<std::unique_ptr<Card>> m_hand;

   // 墓地カード  
   std::vector<std::unique_ptr<Card>> m_cemetery;

   std::mt19937 m_rng{ std::random_device{}() };  

   CardManager() = default;  
   ~CardManager() = default;  

public:  

   // コピー禁止  
   CardManager(const CardManager&) = delete;  
   CardManager& operator = (const CardManager&) = delete;  

   // シングルトンインスタンスの取得  
   static CardManager& GetInstance()  
   {  
       static CardManager instance;  
       return instance;  
   }  

   // デッキの初期化  
   void InitDeck(std::vector<std::unique_ptr<Card>>&& arg_cards);

   // デッキカードの追加  
   void AddDeckCard(std::vector<std::unique_ptr<Card>>&& cards);

   // デッキをシャッフル  
   void DeckShuffle();  

   // デッキから引数分手札に移動  
   void DeckToHand(int arg_drawnum);  

   // 手札からカードを使用  
   CardUseResult UseCard(std::size_t arg_index);

   // 使用したカードを墓地に移動  
   void AddCemeteryCard(std::unique_ptr<Card>&& arg_card);

   // 墓地からデッキに戻す  
   void CemeteryToDeck();  

   // デッキの残り枚数を取得  
   int GetDeckCount() const;  

   // 手札カードを取得  
   int GetHandCard() const;  

   // 墓地にあるカードの確認  
   int GetCemeteryCard() const;  
};
