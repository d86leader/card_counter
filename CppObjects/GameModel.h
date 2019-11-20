#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QPointer>

// Description: model representing game in progress. Counts cards in deck, hand
// and discard, and gives probabilities

class GameModel;
class CommonGameModel : public QAbstractListModel
{
	Q_OBJECT

	struct CardData
	{
		int amount;
		int rowid;
		QString title;

		inline auto operator<(const CardData& other) const -> bool
		{ return amount > other.amount; }
	};

protected:
	QList<CardData> m_cards;
	QPointer<CommonGameModel> m_next;
	QPointer<GameModel> m_parent;
	int m_total;

	// resorting methods. Up = to 0, down = to -1
	auto bubbleUp(int from) -> int;
	auto bubbleDown(int from) -> int;

	enum Roles : int
	{
		Amount = Qt::UserRole
		,Percentage
		,Title
	};

public slots:
	void resetHappened();
	void incrementById(int id);

public:
	CommonGameModel(QObject* parent = nullptr);

	auto data(const QModelIndex& index, int role = Qt::DisplayRole) const
		-> QVariant override;
	auto rowCount(const QModelIndex& parent = QModelIndex()) const
		-> int override;
	//remimplemented to use custom role names
	auto roleNames() const -> QHash<int, QByteArray> override;

	Q_INVOKABLE virtual void pick(int index);
	Q_INVOKABLE virtual void banish(int index);
	Q_INVOKABLE virtual void dropAll();

	friend class GameModel;
};

class ShopModel : public CommonGameModel
{
	Q_OBJECT

public:
	ShopModel(QObject* parent = nullptr);

	auto data(const QModelIndex& index, int role = Qt::DisplayRole) const
		-> QVariant override;
	
	Q_INVOKABLE void pick(int index) override;
	Q_INVOKABLE void banish(int index) override;
	Q_INVOKABLE void dropAll() override;
};

class GameModel : public QObject
{
	Q_OBJECT

	Q_PROPERTY(ShopModel*       shop    MEMBER m_shop    CONSTANT)
	Q_PROPERTY(CommonGameModel* deck    MEMBER m_deck    CONSTANT)
	Q_PROPERTY(CommonGameModel* hand    MEMBER m_hand    CONSTANT)
	Q_PROPERTY(CommonGameModel* discard MEMBER m_discard CONSTANT)
	Q_PROPERTY(int gameId MEMBER m_gameId WRITE setGameId
	           NOTIFY gameIdChanged)
private:
	int m_total;
	int m_gameId;
	QPointer<ShopModel> m_shop;
	QPointer<CommonGameModel>
		m_deck, m_hand, m_discard;

signals:
	void gameIdChanged(int);

public:
	GameModel(QObject* parent = nullptr);

	auto setGameId(int) -> void;

	static auto regType(const char* uri) -> void;

	friend class CommonGameModel;
};
