import QtQuick 2.1
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.1

import "qrc:/qml/components"

import MyCppComponents 1.0

Item {
  id: root

  signal toggleHidBindView

  onToggleHidBindView: hid_bind_view.toggle()

  RCPairRenderer{
    id: renderer

    anchors.fill: parent

    name: rc_pair_cb.currentText

    ComboBox {
      id: rc_pair_cb
      anchors.top: parent.top
      anchors.left: parent.left
      anchors.margins: 5

      width: 128

      opacity: 0.7

      model: rc_name_model
      textRole: "display"
    }

    Button {
      text: "?"
      anchors.top: parent.top
      anchors.right: parent.right
      anchors.margins: 5

      width: height
      opacity: 0.7

      onClicked: hid_bind_view.toggle()
    }

    HidBindingView {
      id: hid_bind_view
      anchors.fill: parent
      anchors.margins: 50
      visible:false

      states: [
        State{
          name: "show"
          PropertyChanges {
            target: hid_bind_view
            visible: true
          }
        }
      ]

      function toggle() {

        if(state === "") state = "show"
        else state = ""

      }
    }

//    TextureViewer {

//      width:  256
//      height: 144

//      anchors.right:  parent.right
//      anchors.bottom: parent.bottom
//    }

  }

}
