#include "CppObjects/DeckBuildModel.h"
#include "defines.h"

#include <QQmlEngine>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include "Backend/database.h"


auto CommonModel::commitPattern(const QString& pattern) -> void
{
	if (m_filter == "")
	{
		setFilter(pattern);
	}
	else
	{
		setFilter(pattern + " and " + m_filter);
	}
}


const QString FilterPattern =
	"exists("
	" select * from card_in_game"
	" where card_id = cards.rowid"
	" and game_id = %1"
	")";
auto CommonModel::createFilter(int gameId) -> QString
{
	return FilterPattern.arg(gameId);
	// this string suits for deckModel
	// for notIncluded it should be prepended by NOT
}

auto DeckBuildModel::setGameId(int id) -> void
{
	m_gameId = id;
	if (m_deckModel)
	{
        m_deckModel->m_filter = DeckModel::createFilter(id);
		// force refiltering
		let pattern = m_deckModel->property("pattern");
		m_deckModel->setPattern(pattern.toString());
	}
	if (m_notIncludedModel)
	{
		m_notIncludedModel->m_filter =
            "not " + DeckModel::createFilter(id);
		// force refiltering
		let pattern = m_notIncludedModel->property("pattern");
		m_notIncludedModel->setPattern(pattern.toString());
	}
	emit gameIdChanged(id);
}


DeckBuildModel::DeckBuildModel(QObject* parent)
	: QObject(parent)
	, m_deckModel(new DeckModel(this))
	, m_notIncludedModel(new NotIncludedModel(this))
{
	m_deckModel->m_sibling = m_notIncludedModel;
	m_notIncludedModel->m_sibling = m_deckModel;

	m_deckModel->setTableName("cards");
	m_notIncludedModel->setTableName("cards");
}


auto DeckModel::moveToSibling(int row) -> void
{
	// means delete from deck
	let cardRowid = data(this->index(row, 0), m_rowidRole);
	let gameId = parent()->property("gameId");

	QSqlQuery q(Database::connect());
	q.prepare(" delete from card_in_game"
	          " where card_id = ? and game_id = ?");
	q.addBindValue(cardRowid);
	q.addBindValue(gameId);
	let r = q.exec();
	if (not r) qDebug() << q.lastError();
	select();
	m_sibling->select();
}


auto NotIncludedModel::moveToSibling(int row) -> void
{
	// means add to deck
	let cardRowid = data(this->index(row, 0), m_rowidRole);
	let gameId = parent()->property("gameId");

	QSqlQuery q(Database::connect());
	q.prepare(" insert into card_in_game (card_id, game_id)"
	          " values (?, ?)");
	q.addBindValue(cardRowid);
	q.addBindValue(gameId);
	let r = q.exec();
	if (not r) qDebug() << q.lastError();
	select();
	m_sibling->select();
}


auto DeckBuildModel::regType(const char* uri) -> void
{
	qmlRegisterType<DeckModel>(uri, 1, 0, "DeckModel");
	qmlRegisterType<NotIncludedModel>(uri, 1, 0, "NotIncludedModel");
	qmlRegisterType<DeckBuildModel>(uri, 1, 0, "DeckBuildModel");
}
