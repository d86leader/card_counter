#include "CppObjects/GameModel.h"
#include "defines.h"
#include <algorithm>
#include <QSqlQuery>
#include <QQmlEngine>
#include "Backend/database.h"


CommonGameModel::CommonGameModel(QObject* parent)
	: QAbstractListModel(parent)
{
}

GameModel::GameModel(QObject* parent)
	: QObject(parent)
	, m_shop(new CommonGameModel(this))
	, m_deck(new CommonGameModel(this))
	, m_hand(new CommonGameModel(this))
	, m_discard(new CommonGameModel(this))
{
	m_shop->m_parent = this;
	m_deck->m_parent = this;
	m_hand->m_parent = this;
	m_discard->m_parent = this;

	m_shop->m_next = m_deck;
	m_deck->m_next = m_hand;
	m_hand->m_next = m_discard;
	m_discard->m_next = m_deck; // note that it's not shop
}


auto CommonGameModel::pick(int index) -> void
{
	qDebug() << "picked";
	m_cards[index].amount -= 1;
	bubbleDown(index);
	// increment in neighbour
	auto& nextCards = m_next->m_cards;
	for (int i = 0; i < nextCards.length(); ++i)
	{
		if (nextCards[i].rowid == m_cards[index].rowid)
		{
			nextCards[i].amount += 1;
			m_next->bubbleUp(i);
			qDebug() << "set next to" << nextCards[i].amount;
			break;
		};
	}
}
auto CommonGameModel::banish(int index) -> void
{
	m_cards[index].amount -= 1;
	bubbleDown(index);
	m_parent->m_total -= 1;
}
auto CommonGameModel::dropAll() -> void
{
	for (auto& card : m_cards)
	{
		// increment neighbour
		for (auto& neighCard : m_next->m_cards)
		{
			if (neighCard.rowid == card.rowid)
			{
				neighCard.amount += card.amount;
			}
		}
		// decrement this
		card.amount = 0;
	}
	// resort neighbour
	auto& nc = m_next->m_cards;
	std::sort(nc.begin(), nc.end());
	m_next->beginResetModel();
	m_next->endResetModel();
	beginResetModel(); endResetModel();
}


auto CommonGameModel::bubbleUp(int from) -> void
{
	int current = from;
	int next = from - 1;
	while (next >= 0 and m_cards[current] < m_cards[next])
	{
		std::swap(m_cards[next], m_cards[current]);
		next -= 1;
		current -= 1;
	}
//	beginMoveRows(QModelIndex(), from, from, QModelIndex(), current);
//	endMoveRows();
	beginResetModel(); endResetModel();
}
auto CommonGameModel::bubbleDown(int from) -> void
{
	int current = from;
	int next = from + 1;
	int last = m_cards.length() - 1;
	while (next <= last and m_cards[next] < m_cards[current])
	{
		std::swap(m_cards[next], m_cards[current]);
		next += 1;
		current += 1;
	}
	beginResetModel(); endResetModel();
}


auto CommonGameModel::rowCount(const QModelIndex&) const -> int
{
	return m_cards.length();
}


auto CommonGameModel::data(const QModelIndex& indexObj, int role) const -> QVariant
{
	if (not indexObj.isValid())  return QVariant();
	let index = indexObj.row();
	if (index > m_cards.length())  return QVariant();

	const auto& card = m_cards[index];

	switch (role)
	{
		case Roles::Amount:
			return card.amount;
		case Roles::Title:
			return card.title;
		case Roles::Percentage:
			return qreal(card.amount) / qreal(m_parent->m_total);
		default:
			return QVariant();
	}
}


auto CommonGameModel::roleNames() const -> QHash<int, QByteArray>
{
	QHash<int, QByteArray> roles;
	roles[Roles::Amount] = "amount";
	roles[Roles::Percentage] = "probability";
	roles[Roles::Title] = "title";
	return roles;
}


auto GameModel::setGameId(int id) -> void
{
	m_gameId = id;
	auto q = QSqlQuery(Database::connect());
	q.prepare(
		" select rowid, title"
		" from cards join card_in_game on card_id = cards.rowid"
		" where game_id = ?"
	);
	q.addBindValue(id);
	let r = q.exec();
	if (r == false) {qDebug() << q.lastError(); return;}

	m_shop->beginResetModel();
	m_deck->beginResetModel();
	m_hand->beginResetModel();
	m_discard->beginResetModel();

	while (q.next())
	{
		let rowid = q.value(0).toInt();
		let title = q.value(1).toString();
		m_shop->   m_cards.push_back({100, rowid, title});
		m_deck->   m_cards.push_back({0, rowid, title});
		m_hand->   m_cards.push_back({0, rowid, title});
		m_discard->m_cards.push_back({0, rowid, title});
	}

	m_shop->endResetModel();
	m_deck->endResetModel();
	m_hand->endResetModel();
	m_discard->endResetModel();
}


auto GameModel::regType(const char* uri) -> void
{
	qmlRegisterType<CommonGameModel>(uri, 1, 0, "CommonGameModel");
	qmlRegisterType<GameModel>(uri, 1, 0, "GameModel");
}
