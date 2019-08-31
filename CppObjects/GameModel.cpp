#include "CppObjects/GameModel.h"
#include "defines.h"
#include <algorithm>
#include <QSqlQuery>
#include <QQmlEngine>
#include "Backend/database.h"


CommonGameModel::CommonGameModel(QObject* parent)
	: QAbstractListModel(parent)
	, m_total(0)
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

	m_shop->m_next = m_hand;
	m_deck->m_next = m_hand;
	m_hand->m_next = m_discard;
	m_discard->m_next = m_deck; // note that it's not shop
}


auto CommonGameModel::incrementById(int id) -> void
{
	for (int i = 0; i < m_cards.length(); ++i)
	{
		if (m_cards[i].rowid == id)
		{
			m_cards[i].amount += 1;
			bubbleUp(i);
			break;
		};
	}
	m_total += 1;
	resetHappened();
}

auto CommonGameModel::pick(int index) -> void
{
	// here's the thing: bubbleDown invalidates the reference, and it also invalidates old index. So we query again
	m_cards[index].amount -= 1;
	index = bubbleDown(index);
	// increment in neighbour
	auto& card = m_cards[index];
	m_next->incrementById(card.rowid);
}
auto CommonGameModel::banish(int index) -> void
{
	m_cards[index].amount -= 1;
	bubbleDown(index);
	m_total -= 1;
	resetHappened();
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
	m_next->m_total += m_total;
	m_total = 0;
	// resort neighbour
	auto& nc = m_next->m_cards;
	std::sort(nc.begin(), nc.end());
	resetHappened();
	m_next->resetHappened();
}


auto CommonGameModel::bubbleUp(int from) -> int
{
	int current = from;
	int next = from - 1;
	while (next >= 0 and m_cards[current] < m_cards[next])
	{
		std::swap(m_cards[next], m_cards[current]);
		next -= 1;
		current -= 1;
	}
	return current;
}
auto CommonGameModel::bubbleDown(int from) -> int
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
	return current;
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
			if (m_total == 0) return 0;
			else return qreal(card.amount) / qreal(m_total);
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
		m_shop->   m_cards.push_back({10000, rowid, title});
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


void CommonGameModel::resetHappened()
{
	beginResetModel();
	endResetModel();
}


ShopModel::ShopModel(QObject* parent)
	: CommonGameModel(parent)
{
}


auto ShopModel::data(const QModelIndex& indexObj, int role) const		-> QVariant
{
	if (not indexObj.isValid())  return QVariant();
	let index = indexObj.row();
	if (index > m_cards.length())  return QVariant();

	const auto& card = m_cards[index];

	switch (role)
	{
		case Roles::Amount:
			return "";
		case Roles::Percentage:
			return "";
		case Roles::Title:
			return card.title;
		default:
			return QVariant();
	}
}

auto ShopModel::pick(int index) -> void
{
	auto& card = m_cards[index];
	m_next->incrementById(card.rowid);
}

auto ShopModel::banish(int) -> void
{
}
auto ShopModel::dropAll() -> void
{
}
