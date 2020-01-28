package satellite

import tornadofx.*

class SimulationView : View() {
    private val myController: MyController by inject()
    override val root = vbox {
        form {
            fieldset {
                field("Type simulation") {
                    combobox(myController.simType, SimulationType.values().asList().asObservable())
                }
                button("Start") {
                    action {
                        myController.messege.set("Start simulation: ${myController.simType}")
                    }
                }
            }


            tableview<Particle> {
                items = myController.particleList
                column("Minimal energy", Particle::minEnergy).makeEditable()
                column("Maximal energy", Particle::maxEnergy).makeEditable()
                column("Step by energy", Particle::stepEnergy).makeEditable()

                contextmenu {
                    item("Add particle") {

                    }
                    item("Delete particle") {
                        action { myController.particleList.remove(selectedItem) }

                    }
                }
            }

            button("Add particle") {
                action {
                    myController.particleList.add(Particle())
                }
            }


        }

    }
}