#include "Controller.h"

Controller::Controller(Serializable::SaveFormat format, QString dataFileName, QString configVis)
{
    //
    // ETAPA 1: Carrega l'escena a visualitzar
    // cas a. A partir de dades virtuals (ja siguin construïdes pel programa o objectes .obj)
    // cas b. A partir de dades geolocalitzades (necessiten un mapeig addicional de les dades a gizmos)

    // S'obté el tipus de factory a utilitzar segons el tipus d'escena (cas a o cas b)
    // definit en el fitxer json de l'escena (veure el camp "typeScene")
    //el tipus de factory pot ser de dades reals o de dades virtuals (dos maneres de crear l'escena)
    auto factory = AbstractFactoryScenes::getInstance().getSceneFactory(format, dataFileName);
    //dataFileName es el OneSphere (informació geomètrica)
    shared_ptr<Scene> scene = factory->createScene(format, dataFileName);

    // ETAPA 2: Configuració de la visualització
    // a. Definició de la càmera
    // b. Definició del viewport
    // c. Set up de l'escena
    auto visualSetup = make_shared<SetUp>();
    //visualSetUp s'encarrega de configurar la càmera, etc
    if (visualSetup->load(format, configVis) ) {
        visualSetup->print(0);

        scene->setBackground(visualSetup->getBackground());
        scene->setMAXDEPTH(visualSetup->getMAXDEPTH());

        scene->setTopBackground(visualSetup->getTopBackground());
        scene->setDownBackground(visualSetup->getDownBackground());

        // TODO: Fase 2: Afegim les llums de l'escena


        // ETAPA 3: Inicialitzacio del Rendering
        // usa un Factory Method per a construir el tipus de render
        //aquesta factory en funció del tipus de raytracing que li demanem (online, image, animacio)
        //crearà un objecte del tipus render per després calcular la imatge a partir de la informació
        render = RayTracingFactory::getInstance()->getRender(visualSetup->getRenderType(), visualSetup->getOutputFile());
        render->setScene(scene);
        render->setCamera(visualSetup->getCamera());

    }
}

void Controller::start(int argc, char **argv) {
    // ETAPA 2: Activació del Rendering
    render->startRendering(argc, argv);
}




