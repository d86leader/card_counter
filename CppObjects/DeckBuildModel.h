#pragma once

#include "CppObjects/SqlTableModel.h"
#include <QPointer>

// Description: used in DeckBuild qml screen. Provides two models: for cards
// not included in deck, and for cards included in deck
class DeckBuildModel;

// The transition from calling it "deck" to "game" is not yet complete, so read
// them as synonyms


// base class for two models
class CommonModel : public SqlTableModel
{
	Q_OBJECT

protected:
	// used for removal communication
	QPointer<CommonModel> m_sibling;
	// used to filter moved items; used in commitPattern
	QString m_filter;
	static auto createFilter(int gameId) -> QString; // this may be prepended by NOT

	using SqlTableModel::setTableName;
	auto commitPattern(const QString&) -> void override;

public:
	using SqlTableModel::SqlTableModel;
	Q_INVOKABLE virtual void moveToSibling(int index) = 0;
};

class DeckModel : public CommonModel
{
	Q_OBJECT

public:
	using CommonModel::CommonModel;

	auto moveToSibling(int) -> void override;

	friend class DeckBuildModel;
};

class NotIncludedModel : public CommonModel
{
	Q_OBJECT

public:
	using CommonModel::CommonModel;

	auto moveToSibling(int) -> void override;

	friend class DeckBuildModel;
};


// container for two models
class DeckBuildModel : public QObject
{
	Q_OBJECT

	Q_PROPERTY(int gameId MEMBER m_gameId WRITE setGameId
	           NOTIFY gameIdChanged)
	Q_PROPERTY(DeckModel* deckModel MEMBER m_deckModel CONSTANT)
	Q_PROPERTY(NotIncludedModel* notIncludedModel MEMBER m_notIncludedModel CONSTANT)

private:
	int m_gameId;
	QPointer<DeckModel> m_deckModel;
	QPointer<NotIncludedModel> m_notIncludedModel;

signals:
	void gameIdChanged(int);

public:
	DeckBuildModel(QObject* parent = nullptr);

	auto setGameId(int) -> void;

	static auto regType(const char* uri) -> void;
};


