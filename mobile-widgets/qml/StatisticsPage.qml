// SPDX-License-Identifier: GPL-2.0
import QtQuick 2.6
import QtQuick.Controls 2.4
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
	StatsManager {
		id: statsManager
	}
	ColumnLayout {
		anchors.fill: parent
		TemplateButton {
			text: qsTr("Plot")
			onClicked: {
				statsManager.doit()
			}
		}
		ComboBox  {
			id: var1
			model: statsManager.var1List
			onCurrentIndexChanged: {
				statsManager.var1Changed(currentIndex)
			}
		}
		ComboBox  {
			id: var1Binner
			model: statsManager.binner1List
			onCurrentIndexChanged: {
				statsManager.var1BinnerChanged(currentIndex)
			}
		}
		ComboBox  {
			id: var2
			model: statsManager.var2List
			onCurrentIndexChanged: {
				statsManager.var2Changed(currentIndex)
			}
		}
		ComboBox  {
			id: var2Binner
			model: statsManager.binner2List
			onCurrentIndexChanged: {
				statsManager.var2BinnerChanged(currentIndex)
			}
		}
		StatsView {
			id: statsView
			Layout.fillWidth: true
			Layout.fillHeight: true
		}
	}
	Component.onCompleted: {
		statsManager.init(statsView, var1)
		console.log("Statistics widget loaded")
	}
}
