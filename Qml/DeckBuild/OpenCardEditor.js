function openEditor(card, openFunction) {
	var item = Qt.resolvedUrl("../CardEdit/Page.qml")
	var props = {"card": card}
	openFunction(item, props)
}
