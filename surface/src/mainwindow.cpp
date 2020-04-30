#include "mainwindow.h"

#include <QScrollArea>
#include <QCheckBox>
#include <QFormLayout>

MainWindow::MainWindow(Q3DSurface *graph, QWidget *container)
{
    m_graph = graph;
    m_container = container;
    m_modifier = new SurfaceGraph(this);

    m_gdName2gdObject[MainWindow::_VanillaGradientDescent] = new VanillaGradientDescent();
    m_gdName2gdObject[MainWindow::_GradientDescentWithMomentum] = new GradientDescentWithMomentum();
    m_gdName2gdObject[MainWindow::_NesterovMomentum] = new NesterovMomentum();
    m_gdName2gdObject[MainWindow::_AdaGrad] = new AdaGrad();
    m_gdName2gdObject[MainWindow::_RMSProp] = new RMSProp();
    m_gdName2gdObject[MainWindow::_Adam] = new Adam();

    m_widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(m_widget);
    QVBoxLayout *rightVLayout = new QVBoxLayout();
    QVBoxLayout *leftVLayout = new QVBoxLayout();
    hLayout->addLayout(leftVLayout);
    hLayout->addWidget(m_container, 1);
    hLayout->addLayout(rightVLayout);

    leftVLayout->setAlignment(Qt::AlignTop);
    rightVLayout->setAlignment(Qt::AlignTop);

    initializeLeftVLayout(leftVLayout);
    initializeRightVLayout(rightVLayout);

    m_widget->setWindowTitle("GDLab");
    m_widget->show();
    //widget->showMaximized();
}

void MainWindow::resetCamera()
{
    m_rotationSliderX->setValue(m_modifier->originalXRotation());
    m_rotationSliderY->setValue(m_modifier->originalYRotation());
    m_zoomSlider->setValue(m_modifier->originalZoomLevel());
}

void MainWindow::updateSliders()
{
    m_rotationSliderX->setValue(m_modifier->xRotation());
    m_rotationSliderY->setValue(m_modifier->yRotation());
}

void MainWindow::updateXRotationSlider(float rotation)
{
    m_rotationSliderX->setValue(rotation);
}

void MainWindow::updateYRotationSlider(float rotation)
{
    m_rotationSliderY->setValue(rotation);
}

void MainWindow::updateZoomLevelSlider(float zoomLevel)
{
    m_zoomSlider->setValue(zoomLevel);
}

QLineEdit* MainWindow::dfdxLineEdit()
{
    return m_dfdxLineEdit;
}

QLineEdit* MainWindow::dfdzLineEdit()
{
    return m_dfdzLineEdit;
}

QLineEdit* MainWindow::fLineEdit()
{
    return m_fLineEdit;
}

bool MainWindow::pointIsOnSurface(QPoint selectedPoint)
{
    return selectedPoint != QPoint(-1, -1);
}

void MainWindow::setSelectedYPoint(double temp)
{
    QJSValue costFunctionEngine = m_modifier->costFunctionEngine();
//    if (!costFunctionEngine.isError()) {
//        float x = m_xSpinBox->value();
//        float z = m_zSpinBox->value();
//        QJSValueList args;
//        args << x << z;
//        float y = costFunctionEngine.call(args).toNumber();
//        m_ySpinBox->setValue(y);

//        m_graph->removeCustomItem(m_previousItem);
//        QImage image = QImage(2, 2, QImage::Format_RGB32);
//        image.fill(Qt::red);
//        QCustom3DItem *item = new QCustom3DItem(":/sphere/sphere", QVector3D(x, y, z),
//                                                QVector3D(0.025f, 0.025f, 0.025f),
//                                                QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, 45.0f),
//                                                image);
//        m_graph->addCustomItem(item);
//        //setPointIsSelected(true);
//    }
}

void MainWindow::setSelectedPoint(QPoint selectedPoint)
{
    if (pointIsOnSurface(selectedPoint)) {
        m_selectedPoint = m_modifier->series()->dataProxy()->itemAt(selectedPoint)->position();
        m_xSpinBox->setValue(m_selectedPoint.x());
        m_ySpinBox->setValue(m_selectedPoint.y());
        m_zSpinBox->setValue(m_selectedPoint.z());
        setPointIsSelected(true);
    }
    else {
       setPointIsSelected(false);
    }
}

QVector3D MainWindow::selectedPoint()
{
    return m_selectedPoint;
}

void MainWindow::initializeInitializationPointRandomly()
{
    // rand() % 5 gives a number between 0 and 4
    int p = rand() % m_modifier->proxy()->columnCount();
    int n = rand() % m_modifier->proxy()->rowCount();
    QPoint selectedPoint = QPoint(n, p);
    m_modifier->series()->setSelectedPoint(selectedPoint);
    m_selectedPoint = m_modifier->series()->dataProxy()->itemAt(selectedPoint)->position();
    m_xSpinBox->setValue(m_selectedPoint.x());
    m_ySpinBox->setValue(m_selectedPoint.y());
    m_zSpinBox->setValue(m_selectedPoint.z());
}

void MainWindow::runGradientDescent()
{
    m_graph->removeCustomItems();
    QCoreApplication::processEvents(QEventLoop::AllEvents); // leave time for Qt to process removeCustomItems

    map<QString, QDoubleSpinBox*>::iterator it;
    for (it = m_keyToSpinBox.begin(); it != m_keyToSpinBox.end(); ++it) {
        QString hyperParameter = it->first.split("_")[1];
        GradientDescent *gradientDescent = m_gdName2gdObject[(MainWindow::GradientDescentMethods)it->first.split("_")[0].toUcs4().first()];
        (gradientDescent->*gradientDescent->hyperParameterToSetter()[hyperParameter])((float)it->second->value());
    }

    if (!pointIsSelected()) {
        initializeInitializationPointRandomly();
    }

    if (m_modifier->partialDerivarivesAreComputed()) {
        map<GradientDescentMethods, QCheckBox*>::iterator it;
        for (it = m_gradientDescentMethodToCheckBox.begin(); it != m_gradientDescentMethodToCheckBox.end(); ++it) {
            if (it->second->isChecked()) {
                GradientDescent *gradientDescent = m_gdName2gdObject[it->first];
                gradientDescent->initialize(m_modifier, selectedPoint());
                gradientDescent->run();
                plotPoints(gradientDescent);
            }
        }
    }
    else {
        QMessageBox::critical(this, tr("Error"), tr("Don't forget to compute partial derivative before running gradient descent"));
    }
}

Q3DSurface* MainWindow::graph()
{
    return m_graph;
}

SurfaceGraph* MainWindow::modifier()
{
    return m_modifier;
}

void MainWindow::setPointIsSelected(bool pointIsSelected)
{
    m_pointIsSelected = pointIsSelected;
}

bool MainWindow::pointIsSelected()
{
    return m_pointIsSelected;
}

QPushButton* MainWindow::cameraPOVButton()
{
    return m_cameraPOVButton;
}

void MainWindow::plotPoints(GradientDescent *gradientDescentMethod)
{
    vector<QVector3D> pointsTable = gradientDescentMethod->pointsTable();
    QImage image = QImage(2, 2, QImage::Format_RGB32);
    image.fill(gradientDescentMethod->color());
    for (vector<QVector3D>::iterator it=pointsTable.begin(); it!=pointsTable.end(); ++it) {
        QCustom3DItem *item = new QCustom3DItem(":/sphere/sphere", *it,
                                                QVector3D(0.02f, 0.02f, 0.02f),
                                                gradientDescentMethod->rotation(),
                                                image);
        m_graph->addCustomItem(item);
    }
}

void MainWindow::togglePoints(GradientDescent *gradientDescentMethod, bool showCurve)
{
    QList<QCustom3DItem *>::iterator it;
    QList<QCustom3DItem *> list = m_graph->customItems();
    for (it = list.begin(); it != list.end(); ++it) {
        if ((*it)->rotation() == gradientDescentMethod->rotation()) {
            (*it)->setVisible(showCurve);
        }
    }
}

void MainWindow::toggleCurve(bool checked)
{
    int curve = sender()->property("ID").toInt();
    GradientDescent *gradientDescentMethod = m_gdName2gdObject[MainWindow::GradientDescentMethods(curve)];
    togglePoints(gradientDescentMethod, !checked);
}

QString MainWindow::key(MainWindow::GradientDescentMethods gradientDescentMethod, QString hyperParameter)
{
    return QString("%1_%2").arg(QString(gradientDescentMethod), hyperParameter);
}

void MainWindow::initializeRightVLayout(QVBoxLayout *rightVLayout)
{
    QGroupBox *checkGroup = new QGroupBox("Selection des plages a importer:");
    checkGroup->setFlat(true);

    // QScrollArea doit être dans un layout pour prendre toute la place
    // du QGroupBox
    QVBoxLayout *groupLayout = new QVBoxLayout(checkGroup);

    // Crée et ajoute le scrollArea au groupe
    QScrollArea *scrollArea = new QScrollArea(checkGroup);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    scrollArea->setSizePolicy(sizePolicy);
    groupLayout->addWidget(scrollArea);

    // Cree le widget qui va contenir les checkboxes...
    QWidget *scrolledWidget = new QWidget(scrollArea);
    // ... et son layout
    QVBoxLayout *scrolledLayout = new QVBoxLayout(scrolledWidget);

    m_functionList = new QComboBox();
    m_functionList->addItem(tr("Inclined Taco Shell"));
    m_functionList->addItem(tr("Sqrt & Sin"));
    m_functionList->addItem(tr("Narrow Saddle"));
    m_functionList->addItem(tr("NonConvex"));
    m_functionList->addItem(tr("Wide Saddle"));

    QLabel *fLabel = new QLabel(tr("y = f(x, z)"));
    m_fLineEdit = new QLineEdit();
    m_fLineEdit->setEnabled(true);

    m_computePartialDerivativesButton = new QPushButton();
    m_computePartialDerivativesButton->setText(tr("Compute partial derivatives"));

    QLabel *dfdxLabel = new QLabel(tr("df/dx"));
    m_dfdxLineEdit = new QLineEdit();
    m_dfdxLineEdit->setEnabled(true);

    QLabel *dfdzLabel = new QLabel(tr("df/dz"));
    m_dfdzLineEdit = new QLineEdit();
    m_dfdzLineEdit->setEnabled(true);

    m_xSpinBox = new QDoubleSpinBox();
    m_xSpinBox->setLocale(QLocale::English);
    m_xSpinBox->setRange(-8.0f, 8.0f);
    m_xSpinBox->setSingleStep(0.05);
    m_ySpinBox = new QDoubleSpinBox();
    m_ySpinBox->setLocale(QLocale::English);
    m_ySpinBox->setDisabled(true);
    m_zSpinBox = new QDoubleSpinBox();
    m_zSpinBox->setLocale(QLocale::English);
    m_zSpinBox->setRange(-8.0f, 8.0f);
    m_zSpinBox->setSingleStep(0.05);
    QGridLayout *spinBoxLayout = new QGridLayout();

    spinBoxLayout->addWidget(m_xSpinBox, 0, 0);
    spinBoxLayout->addWidget(new QLabel("x"), 0, 1);
    spinBoxLayout->addWidget(m_ySpinBox, 0, 2);
    spinBoxLayout->addWidget(new QLabel("y"), 0, 3);
    spinBoxLayout->addWidget(m_zSpinBox, 0, 4);
    spinBoxLayout->addWidget(new QLabel("z"), 0, 5);

    m_runGradientDescentButton = new QPushButton();
    m_runGradientDescentButton->setText(tr("Run Gradient Descent"));

    QGroupBox *functionGroupBox = new QGroupBox(tr("Function"));
    QVBoxLayout *functionLayout = new QVBoxLayout;
    functionLayout->addWidget(m_functionList);
    QVBoxLayout *fLayout = new QVBoxLayout();
    fLayout->addWidget(fLabel);
    fLayout->addWidget(m_fLineEdit);
    functionLayout->addLayout(fLayout);
    functionLayout->addWidget(m_computePartialDerivativesButton);
    QVBoxLayout *dfdxLayout = new QVBoxLayout();
    dfdxLayout->addWidget(dfdxLabel);
    dfdxLayout->addWidget(m_dfdxLineEdit);
    functionLayout->addLayout(dfdxLayout);
    QVBoxLayout *dfdzLayout = new QVBoxLayout();
    dfdzLayout->addWidget(dfdzLabel);
    dfdzLayout->addWidget(m_dfdzLineEdit);
    functionLayout->addLayout(dfdzLayout);
    functionLayout->addWidget(m_runGradientDescentButton);
    functionLayout->addLayout(spinBoxLayout);
    functionGroupBox->setLayout(functionLayout);

    m_axisMinSliderX = new QSlider(Qt::Horizontal);
    m_axisMinSliderX->setMinimum(0);
    m_axisMinSliderX->setTickInterval(1);
    m_axisMinSliderX->setEnabled(true);
    m_axisMaxSliderX = new QSlider(Qt::Horizontal);
    m_axisMaxSliderX->setMinimum(1);
    m_axisMaxSliderX->setTickInterval(1);
    m_axisMaxSliderX->setEnabled(true);
    m_axisMinSliderZ = new QSlider(Qt::Horizontal);
    m_axisMinSliderZ->setMinimum(0);
    m_axisMinSliderZ->setTickInterval(1);
    m_axisMinSliderZ->setEnabled(true);
    m_axisMaxSliderZ = new QSlider(Qt::Horizontal);
    m_axisMaxSliderZ->setMinimum(1);
    m_axisMaxSliderZ->setTickInterval(1);
    m_axisMaxSliderZ->setEnabled(true);

    m_themeList = new QComboBox();
    m_themeList->addItem(tr("Qt"));
    m_themeList->addItem(tr("Primary Colors"));
    m_themeList->addItem(tr("Digia"));
    m_themeList->addItem(tr("Stone Moss"));
    m_themeList->addItem(tr("Army Blue"));
    m_themeList->addItem(tr("Retro"));
    m_themeList->addItem(tr("Ebony"));
    m_themeList->addItem(tr("Isabelle"));

    QGroupBox *themeGroupBox = new QGroupBox(tr("Theme"));

    const int pixMapWidth = 150;
    const int pixMapHeight = 15;
    QLinearGradient uniformColorMap(0, 0, pixMapWidth, pixMapHeight);
    uniformColorMap.setColorAt(0.0, Qt::gray);
    QPixmap pm(pixMapWidth, pixMapHeight);
    QPainter pmp(&pm);
    pmp.setBrush(QBrush(uniformColorMap));
    pmp.setPen(Qt::NoPen);
    pmp.drawRect(0, 0, pixMapWidth, pixMapHeight);

    m_colormapList = new QComboBox();
    m_colormapList->addItem(QIcon(pm), tr("Default"));

    static map<tinycolormap::ColormapType, QString> colormap2string = {
       {tinycolormap::ColormapType::Heat, "Heat"},
       {tinycolormap::ColormapType::Jet, "Jet"},
       {tinycolormap::ColormapType::Hot, "Hot"},
       {tinycolormap::ColormapType::Gray, "Gray"},
       {tinycolormap::ColormapType::Magma, "Magma"},
       {tinycolormap::ColormapType::Inferno, "Inferno"},
       {tinycolormap::ColormapType::Plasma, "Plasma"},
       {tinycolormap::ColormapType::Viridis, "Viridis"},
       {tinycolormap::ColormapType::Cividis, "Cividis"},
       {tinycolormap::ColormapType::Github, "Github"}
    };

    for(map<tinycolormap::ColormapType, QString>::iterator it = colormap2string.begin(); it != colormap2string.end(); ++it)
    {
        QLinearGradient gr(0, 0, pixMapWidth, pixMapHeight);
        for (float i = 0; i<=1; i += 0.1) {
            const QColor color = tinycolormap::GetColor(i, it->first).ConvertToQColor();
            gr.setColorAt(i, color);
        }

        pmp.setBrush(QBrush(gr));
        pmp.drawRect(0, 0, pixMapWidth, pixMapHeight);
        m_colormapList->addItem(QIcon(pm), it->second);
    }

    m_colormapList->setIconSize(QSize(pixMapWidth, pixMapHeight));

    m_surfaceList = new QComboBox();
    m_surfaceList->addItem(tr("WireFrame"));
    m_surfaceList->addItem(tr("Surface"));
    m_surfaceList->addItem(tr("SurfaceAndWireframe"));

    QVBoxLayout *themeLayout = new QVBoxLayout();
    themeLayout->addWidget(m_surfaceList);
    themeLayout->addWidget(m_themeList);
    themeLayout->addWidget(m_colormapList);
    themeGroupBox->setLayout(themeLayout);

    QGroupBox *axisXRangeHBoxGroupBox = new QGroupBox(tr("Column range (X slider)"));
    QVBoxLayout *axisXRangeVBox = new QVBoxLayout();
    axisXRangeVBox->addWidget(m_axisMinSliderX);
    axisXRangeVBox->addWidget(m_axisMaxSliderX);
    axisXRangeHBoxGroupBox->setLayout(axisXRangeVBox);

    QGroupBox *axisZRangeHBoxGroupBox = new QGroupBox(tr("Row range (Z slider)"));
    QVBoxLayout *axisZRangeVBox = new QVBoxLayout();
    axisZRangeVBox->addWidget(m_axisMinSliderZ);
    axisZRangeVBox->addWidget(m_axisMaxSliderZ);
    axisZRangeHBoxGroupBox->setLayout(axisZRangeVBox);

    scrolledLayout->addWidget(functionGroupBox);
    scrolledLayout->addWidget(new QLabel(tr("View")));
    m_viewList = new QComboBox();
    m_viewList->addItem(tr("3D Surface"));
    m_viewList->addItem(tr("Row Slice"));
    m_viewList->addItem(tr("Column Slice"));
    scrolledLayout->addWidget(m_viewList);
    scrolledLayout->addWidget(axisXRangeHBoxGroupBox);
    scrolledLayout->addWidget(axisZRangeHBoxGroupBox);

    scrolledLayout->addWidget(themeGroupBox);

    m_rotationSliderX = new QSlider(Qt::Horizontal);
    m_rotationSliderX->setTickInterval(30);
    m_rotationSliderX->setMinimum(-180);
    m_rotationSliderX->setValue(m_modifier->originalXRotation());
    m_rotationSliderX->setMaximum(180);
    m_rotationSliderY = new QSlider(Qt::Horizontal);
    m_rotationSliderY->setTickInterval(10);
    m_rotationSliderY->setMinimum(0);
    m_rotationSliderY->setValue(m_modifier->originalYRotation());
    m_rotationSliderY->setMaximum(90);
    m_zoomSlider = new QSlider(Qt::Horizontal);
    m_zoomSlider->setTickInterval(10);
    m_zoomSlider->setMinimum(0);
    m_zoomSlider->setValue(m_modifier->originalZoomLevel());
    m_zoomSlider->setMaximum(600);

    m_resetCameraButton = new QPushButton();
    m_resetCameraButton->setText(tr("Reset view"));

    m_cameraPOVButton = new QPushButton();

    QGridLayout *gridLayout = new QGridLayout();

    QVBoxLayout *buttonsVBox = new QVBoxLayout();
    buttonsVBox->addWidget(m_cameraPOVButton, 0, Qt::AlignTop);
    buttonsVBox->addWidget(m_resetCameraButton, 0, Qt::AlignTop);
    gridLayout->addLayout(buttonsVBox, 0, 0);

    QGroupBox *zoomGroupBox = new QGroupBox(tr("Zoom"));
    QHBoxLayout *zoomHBox = new QHBoxLayout();
    zoomHBox->addWidget(m_zoomSlider, 0, Qt::AlignTop);
    zoomGroupBox->setLayout(zoomHBox);
    gridLayout->addWidget(zoomGroupBox, 1, 0);

    QGroupBox *rotateXGroupBox = new QGroupBox(tr("Rotate horizontally"));
    QHBoxLayout *rotateXHBox = new QHBoxLayout();
    rotateXHBox->addWidget(m_rotationSliderX, 0, Qt::AlignTop);
    rotateXGroupBox->setLayout(rotateXHBox);
    gridLayout->addWidget(rotateXGroupBox, 2, 0);

    QGroupBox *rotateYGroupBox = new QGroupBox(tr("Rotate vertically"));
    QHBoxLayout *rotateYHBox = new QHBoxLayout();
    rotateYHBox->addWidget(m_rotationSliderY, 0, Qt::AlignTop);
    rotateYGroupBox->setLayout(rotateYHBox);
    gridLayout->addWidget(rotateYGroupBox, 3, 0);
    scrolledLayout->addLayout(gridLayout);

    scrollArea->setWidget(scrolledWidget);
    rightVLayout->addWidget(scrollArea);

    QObject::connect(m_fLineEdit, &QLineEdit::textEdited,
                     m_modifier, &SurfaceGraph::drawModel);
    QObject::connect(m_axisMinSliderX, &QSlider::valueChanged,
                     m_modifier, &SurfaceGraph::adjustXMin);
    QObject::connect(m_axisMaxSliderX, &QSlider::valueChanged,
                     m_modifier, &SurfaceGraph::adjustXMax);
    QObject::connect(m_axisMinSliderZ, &QSlider::valueChanged,
                     m_modifier, &SurfaceGraph::adjustZMin);
    QObject::connect(m_axisMaxSliderZ, &QSlider::valueChanged,
                     m_modifier, &SurfaceGraph::adjustZMax);
    QObject::connect(m_themeList, SIGNAL(activated(int)),
                     m_modifier, SLOT(changeTheme(int)));
    QObject::connect(m_rotationSliderX, &QSlider::valueChanged, m_modifier, &SurfaceGraph::rotateX);
    QObject::connect(m_rotationSliderY, &QSlider::valueChanged, m_modifier, &SurfaceGraph::rotateY);
    QObject::connect(m_zoomSlider, &QSlider::valueChanged, m_modifier, &SurfaceGraph::zoom);
    QObject::connect(m_resetCameraButton, SIGNAL(clicked()), this,
                     SLOT(resetCamera()));
    QObject::connect(m_cameraPOVButton, &QPushButton::pressed, m_modifier,
                     &SurfaceGraph::changePresetCamera);
    QObject::connect(m_computePartialDerivativesButton, &QPushButton::pressed,
                     m_modifier, &SurfaceGraph::computePartialDerivatives);
    QObject::connect(m_functionList, SIGNAL(currentIndexChanged(int)),
                     m_modifier, SLOT(changeCostFunction(int)));
    QObject::connect(m_viewList, SIGNAL(currentIndexChanged(int)),
                     m_modifier, SLOT(changeSelectionMode(int)));
    QObject::connect(m_colormapList, SIGNAL(activated(int)),
                     m_modifier, SLOT(changeColormap(int)));
    QObject::connect(m_surfaceList, SIGNAL(currentIndexChanged(int)),
                     m_modifier, SLOT(changeSurface(int)));

    QObject::connect(m_cameraPOVButton, SIGNAL(clicked()), this,
                     SLOT(updateSliders()));

    QObject::connect(m_graph->scene()->activeCamera(), SIGNAL(xRotationChanged(float)), this,
                     SLOT(updateXRotationSlider(float)));
    QObject::connect(m_graph->scene()->activeCamera(), SIGNAL(yRotationChanged(float)), this,
                     SLOT(updateYRotationSlider(float)));
    QObject::connect(m_graph->scene()->activeCamera(), SIGNAL(zoomLevelChanged(float)), this,
                     SLOT(updateZoomLevelSlider(float)));

    QObject::connect(m_modifier->series(), SIGNAL(selectedPointChanged(QPoint)), this,
    SLOT(setSelectedPoint(QPoint))); // Problem : does not send signal when click is perfomed at same location
/*    QObject::connect(m_graph->scene(), SIGNAL(selectionQueryPositionChanged(QPoint)), this,
                     SLOT(setSelectedPoint(QPoint)), Qt::UniqueConnection);*/ // Problem : calls slot twice for every signal sent

    QObject::connect(m_runGradientDescentButton, SIGNAL(clicked()), this,
                     SLOT(runGradientDescent()));

    QObject::connect(m_xSpinBox, SIGNAL(valueChanged(double)), this,
                     SLOT(setSelectedYPoint(double)));

    QObject::connect(m_zSpinBox, SIGNAL(valueChanged(double)), this,
                     SLOT(setSelectedYPoint(double)));

    m_modifier->setAxisMinSliderX(m_axisMinSliderX);
    m_modifier->setAxisMaxSliderX(m_axisMaxSliderX);
    m_modifier->setAxisMinSliderZ(m_axisMinSliderZ);
    m_modifier->setAxisMaxSliderZ(m_axisMaxSliderZ);

    m_functionList->setCurrentIndex(1);
    m_themeList->activated(2);
    m_themeList->setCurrentIndex(2);
    m_colormapList->activated(0);
    m_surfaceList->setCurrentIndex(2);
    m_viewList->setCurrentIndex(0);

    m_modifier->changePresetCamera();
}

void MainWindow::initializeLeftVLayout(QVBoxLayout *leftVLayout)
{
    QGroupBox *checkGroup = new QGroupBox("Selection des plages a importer:");
    checkGroup->setFlat(true);

    // QScrollArea doit être dans un layout pour prendre toute la place
    // du QGroupBox
    QVBoxLayout *groupLayout = new QVBoxLayout(checkGroup);

    // Crée et ajoute le scrollArea au groupe
    QScrollArea *scrollArea = new QScrollArea(checkGroup);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    scrollArea->setSizePolicy(sizePolicy);
    groupLayout->addWidget(scrollArea);

    // Cree le widget qui va contenir les checkboxes...
    QWidget *scrolledWidget = new QWidget(scrollArea);

    QVBoxLayout *scrolledLayout = new QVBoxLayout(scrolledWidget);

    // -----
    m_toggleCurvesButton = new QPushButton("Toggle curves");
    scrolledLayout->addWidget(m_toggleCurvesButton);
    QObject::connect(m_toggleCurvesButton, SIGNAL(pressed()),
                     this, SLOT(toggleCurves()));
    // -----
    m_toggleCurvesButton = new QPushButton("Reset values");
    scrolledLayout->addWidget(m_toggleCurvesButton);
    QObject::connect(m_toggleCurvesButton, SIGNAL(pressed()),
                     this, SLOT(resetValues()));
    // -----

    const int width = 20;
    const int height = 2;
    QPixmap pm(width, height);
    QPainter pmp(&pm);
    pmp.setPen(Qt::NoPen);

    map<GradientDescentMethods, GradientDescent*>::iterator it;
    for (it = m_gdName2gdObject.begin(); it != m_gdName2gdObject.end(); ++it) {

        // -----
        QFormLayout *formLayout = new QFormLayout();
        QCheckBox *runGDAlgortihmsCheckbox = new QCheckBox();
        runGDAlgortihmsCheckbox->setChecked(true);
        m_gradientDescentMethodToCheckBox[it->first] = runGDAlgortihmsCheckbox;

        pmp.setBrush(QBrush(it->second->color()));
        pmp.drawRect(0, 0, width, height);
        QPushButton *button = new QPushButton(QIcon(pm), it->second->name());
        button->setIconSize(QSize(width, height));
        button->setStyleSheet("Text-align: left; padding: 4px 0px 4px 4px;");
        button->setProperty("ID", it->first);
        button->setCheckable(true);
        QObject::connect(button, SIGNAL(toggled(bool)),
                         this, SLOT(toggleCurve(bool)));
        m_gradientDescentMethodToPushButton[it->first] = button;

        formLayout->addRow(runGDAlgortihmsCheckbox, button);

        scrolledLayout->addLayout(formLayout);
        // -----

        // -----
        QVBoxLayout *hyperParameterVLayout = new QVBoxLayout();

        int i = 0;
        int j = 0;
        QGridLayout *gridLayout = new QGridLayout();
        for (QString hyperParameter : it->second->hyperParameters()) {
           if (j == 2) {
                i++;
                j = 0;
           }

           QDoubleSpinBox *spinBox = new QDoubleSpinBox();
           spinBox->setLocale(QLocale::English);
           spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
           if (hyperParameter == "nIterMax") {
               spinBox->setRange(0, 100000);
               spinBox->setDecimals(0);
           }
           else {
               spinBox->setRange(-100, 100);
               spinBox->setDecimals(4);
           }
           setPredefinedValues(spinBox, hyperParameter);

           gridLayout->addWidget(spinBox, i, j);
           j++;
           m_keyToSpinBox[key(it->first, hyperParameter)] = spinBox;

           gridLayout->addWidget(new QLabel(hyperParameter), i, j);
           j++;
           hyperParameterVLayout->addLayout(gridLayout);
        }

        // -----
        scrolledLayout->addLayout(hyperParameterVLayout);
    }
    scrollArea->setWidget(scrolledWidget);
    leftVLayout->addWidget(scrollArea);
}

void MainWindow::setPredefinedValues(QDoubleSpinBox *spinBox, QString hyperParameter)
{
    if (hyperParameter == "lr") {
        spinBox->setValue(1e-3);
    }
    else if (hyperParameter == "tol") {
        spinBox->setValue(0.005);
    }
    else if (hyperParameter == "nIterMax") {
        spinBox->setValue(10000);
    }
    else if (hyperParameter == "beta1") {
        spinBox->setValue(0.9);
    }
    else if (hyperParameter == "beta2") {
        spinBox->setValue(0.999);
    }
    else if (hyperParameter == "decayRate") {
        spinBox->setValue(0.9);
    }
    else if (hyperParameter == "rho") {
        spinBox->setValue(0.9);
    }
    else {
        qDebug() << "Problem in setPredefinedValues";
    }
}

void MainWindow::toggleCurves()
{
    map<GradientDescentMethods, QPushButton*>::iterator it;
    for (it = m_gradientDescentMethodToPushButton.begin(); it != m_gradientDescentMethodToPushButton.end(); ++it) {
        QPushButton *button = it->second;
        button->setChecked(!button->isChecked());
    }
}

void MainWindow::resetValues()
{
    map<QString, QDoubleSpinBox*>::iterator it;
    for (it = m_keyToSpinBox.begin(); it != m_keyToSpinBox.end(); ++it) {
        QString hyperParameter = it->first.split("_")[1];
        QDoubleSpinBox *spinBox = it->second;
        setPredefinedValues(spinBox, hyperParameter);
    }
}
