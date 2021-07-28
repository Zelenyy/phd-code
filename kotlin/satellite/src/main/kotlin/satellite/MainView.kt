package satellite

import javafx.beans.property.SimpleDoubleProperty
import tornadofx.*




class MainView : View() {
    private val myController: MyController by inject()
    override val root = borderpane {

        top = menubar {

            menu("Detector Configurations") {
                item("Add specific configuration")
                item("Create new optimizer")
                menu("Open Recent") {
                }

            }
            menu("Settings") {
            }
            menu("About") {

            }
        }

        center = vbox {
            tabpane {
                tab("Geometry") {
                    val geo = find<GeometryEditorView>()
                    content = geo.root
                }
                tab("Simulation") {
                    val sim = find<SimulationView>()
                    content = sim.root
                }
                tab("Reconstruction") {

                }
            }
            scrollpane {
                textarea {
                    this.isEditable = false
                    myController.messege.onChange {
                        appendText(it ?: "NULL")
                        appendText("\n")
                    }
                }
            }

        }
    }
}