// SPDX-License-Identifier: GPL-2.0
import QtQuick 2.6
import QtQuick.Layouts 1.2
import org.subsurfacedivelog.mobile 1.0
import org.kde.kirigami 2.4 as Kirigami

Kirigami.Page {
	id: statisticsPage
	objectName: "StatisticsPage"
	title: qsTr("Statistics")
	leftPadding: 0
	topPadding: 0
	rightPadding: 0
	bottomPadding: 0
	width: rootItem.colWidth
	ColumnLayout {
		anchors.fill: parent
		TemplateButton {
			text: qsTr("Plot")
			onClicked: {
				statsManager.doit()
			}
		}
		StatsView {
			id: statsView
			Layout.fillWidth: true
			Layout.fillHeight: true
		}
	}
	StatsManager {
		id: statsManager
	}
	Component.onCompleted: {
		statsManager.init(statsView)
		console.log("Statistics widget loaded")
	}
}
