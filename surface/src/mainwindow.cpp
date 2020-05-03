#include "mainwindow.h"

MainWindow::MainWindow(Q3DSurface *graph, QWidget *container)
{
    m_graph = graph;
    m_container = container;
    m_modifier = new SurfaceGraph(this);

    m_gradientDescentMethodToGradientDescent["VanillaGradientDescent"] = new VanillaGradientDescent();
    m_gradientDescentMethodToGradientDescent["GradientDescentWithMomentum"] = new GradientDescentWithMomentum();
    m_gradientDescentMethodToGradientDescent["NesterovMomentum"] = new NesterovMomentum();
    m_gradientDescentMethodToGradientDescent["AdaGrad"] = new AdaGrad();
    m_gradientDescentMethodToGradientDescent["RMSProp"] = new RMSProp();
    m_gradientDescentMethodToGradientDescent["Adam"] = new Adam();

    int i = 0;
    map<QString, GradientDescent*>::iterator it;
    for (it = m_gradientDescentMethodToGradientDescent.begin(); it != m_gradientDescentMethodToGradientDescent.end(); ++it) {
        m_gradientDescentMethodToInt[it->first] = i;
        m_intToGradientDescentMethod[i] = it->first;
        i++;
    }

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
    m_widget->showMaximized();
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

QLineEdit* MainWindow::costFunctionLineEdit()
{
    return m_costFunctionLineEdit;
}

bool MainWindow::pointIsOnSurface(QPoint selectedPoint)
{
    return selectedPoint != QPoint(-1, -1);
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

    // take QDoubleSpinBox values and set them in the respective gradient descent methods
    map<QString, QDoubleSpinBox*>::iterator it;
    for (it = m_keyToSpinBox.begin(); it != m_keyToSpinBox.end(); ++it) {
        QString gradientDescentMethod = it->first.split("_")[0];
        QString hyperParameter = it->first.split("_")[1];        
        GradientDescent *gradientDescent = m_gradientDescentMethodToGradientDescent[gradientDescentMethod];
        gradientDescent->setHyperParameterValue(hyperParameter, it->second->value());
    }

    if (!pointIsSelected()) {
        initializeInitializationPointRandomly();
        setPointIsSelected(false);
    }

    if (m_modifier->partialDerivarivesAreComputed()) {
        map<QString, QCheckBox*>::iterator it;
        for (it = m_gradientDescentMethodToCheckBox.begin(); it != m_gradientDescentMethodToCheckBox.end(); ++it) {
            if (it->second->isChecked()) {
                GradientDescent *gradientDescent = m_gradientDescentMethodToGradientDescent[it->first];
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
    vector<QVector3D> points = gradientDescentMethod->points();
    QImage image = QImage(2, 2, QImage::Format_RGB32);
    image.fill(gradientDescentMethod->color());
    for (vector<QVector3D>::iterator it=points.begin(); it!=points.end(); ++it) {
        QCustom3DItem *item = new QCustom3DItem(":/sphere.obj", *it,
                                                QVector3D(0.02f, 0.02f, 0.02f),
                                                QQuaternion::fromAxisAndAngle(0.0f, 0.0f, 0.0f, 0.0f),
                                                image);
        item->setProperty("id", gradientDescentMethod->id());
        m_graph->addCustomItem(item);
    }
}

void MainWindow::togglePoints(GradientDescent *gradientDescentMethod, bool showCurve)
{
    QList<QCustom3DItem *>::iterator it;
    QList<QCustom3DItem *> list = m_graph->customItems();
    for (it = list.begin(); it != list.end(); ++it) {
        if ((*it)->property("id").toInt() == gradientDescentMethod->id()) {
            (*it)->setVisible(showCurve);
        }
    }
}

void MainWindow::toggleCurve(bool checked)
{
    int curve = sender()->property("id").toInt();
    QString gradientDescentMethod = m_intToGradientDescentMethod[curve];
    GradientDescent *gradientDescent = m_gradientDescentMethodToGradientDescent[gradientDescentMethod];
    togglePoints(gradientDescent, !checked);
}

QString MainWindow::key(QString gradientDescentMethod, QString hyperParameter)
{
    return QString("%1_%2").arg(gradientDescentMethod, hyperParameter);
}

QScrollArea* MainWindow::initializeScrollArea(QScrollArea *scrollArea)
{
    QGroupBox *scrollGroupBox = new QGroupBox();

    // QScrollArea must be in a layout to take up all the space
    // of the QGroupBox
    QVBoxLayout *groupLayout = new QVBoxLayout(scrollGroupBox);

    // Create and add the scrollArea to the group
    scrollArea = new QScrollArea(scrollGroupBox);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    scrollArea->setSizePolicy(sizePolicy);
    groupLayout->addWidget(scrollArea);

    return scrollArea;
}

void MainWindow::initializeRightVLayout(QVBoxLayout *rightVLayout)
{
    QScrollArea *scrollArea = 0;
    scrollArea = initializeScrollArea(scrollArea);

    // Create a widget that will contain other widgets and layouts
    QWidget *scrollWidget = new QWidget(scrollArea);
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);

    // -----
    QGroupBox *mathsGroupBox = new QGroupBox(tr("Maths"));
    QVBoxLayout *mathsVBox = new QVBoxLayout();

    QLabel *costFunctionLabel = new QLabel("y = f(x, z)", m_widget);
    mathsVBox->addWidget(costFunctionLabel);

    m_costFunctionList = new QComboBox();
    m_costFunctionList->addItem(tr("Inclined Taco Shell"));
    m_costFunctionList->addItem(tr("Sqrt & Sin"));
    m_costFunctionList->addItem(tr("Narrow Saddle"));
    m_costFunctionList->addItem(tr("NonConvex"));
    m_costFunctionList->addItem(tr("Wide Saddle"));
    mathsVBox->addWidget(m_costFunctionList);

    m_costFunctionLineEdit = new QLineEdit(m_widget);
    mathsVBox->addWidget(m_costFunctionLineEdit);

    m_computePartialDerivativesButton = new QPushButton(m_widget);
    m_computePartialDerivativesButton->setText(tr("Compute partial derivatives"));
    mathsVBox->addWidget(m_computePartialDerivativesButton);

    QLabel *dfdxLabel = new QLabel("df/dx", m_widget);
    mathsVBox->addWidget(dfdxLabel);

    m_dfdxLineEdit = new QLineEdit(m_widget);
    m_dfdxLineEdit->setReadOnly(true);
    mathsVBox->addWidget(m_dfdxLineEdit);

    QLabel *dfdzLabel = new QLabel("df/dz", m_widget);
    mathsVBox->addWidget(dfdzLabel);

    m_dfdzLineEdit = new QLineEdit(m_widget);
    m_dfdzLineEdit->setReadOnly(true);
    mathsVBox->addWidget(m_dfdzLineEdit);

    m_runGradientDescentButton = new QPushButton(m_widget);
    m_runGradientDescentButton->setText(tr("Run Gradient Descent"));
    mathsVBox->addWidget(m_runGradientDescentButton);

    m_xSpinBox = new QDoubleSpinBox(m_widget);
    m_xSpinBox->setLocale(QLocale::English);
    m_xSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_xSpinBox->setReadOnly(true);

    m_ySpinBox = new QDoubleSpinBox(m_widget);
    m_ySpinBox->setLocale(QLocale::English);
    m_ySpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_ySpinBox->setReadOnly(true);

    m_zSpinBox = new QDoubleSpinBox(m_widget);
    m_zSpinBox->setLocale(QLocale::English);
    m_zSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_zSpinBox->setReadOnly(true);

    QGridLayout *spinBoxGrid = new QGridLayout(m_widget);
    spinBoxGrid->addWidget(m_xSpinBox, 0, 0);
    spinBoxGrid->addWidget(new QLabel("x", m_widget), 0, 1);
    spinBoxGrid->addWidget(m_ySpinBox, 0, 2);
    spinBoxGrid->addWidget(new QLabel("y", m_widget), 0, 3);
    spinBoxGrid->addWidget(m_zSpinBox, 0, 4);
    spinBoxGrid->addWidget(new QLabel("z", m_widget), 0, 5);
    mathsVBox->addLayout(spinBoxGrid);

    mathsGroupBox->setLayout(mathsVBox);
    scrollLayout->addWidget(mathsGroupBox);
    // -----

    // -----
    QGroupBox *viewGroupBox = new QGroupBox(tr("View"));
    QVBoxLayout *viewVBox = new QVBoxLayout();
    m_viewList = new QComboBox();
    m_viewList->addItem(tr("3D"));
    m_viewList->addItem(tr("Row Slice"));
    m_viewList->addItem(tr("Column Slice"));
    viewVBox->addWidget(m_viewList);

    m_rotationSliderX = new QSlider(Qt::Horizontal, m_widget);
    m_rotationSliderX->setTickInterval(30);
    m_rotationSliderX->setMinimum(-180);
    m_rotationSliderX->setValue(m_modifier->originalXRotation());
    m_rotationSliderX->setMaximum(180);
    m_rotationSliderY = new QSlider(Qt::Horizontal, m_widget);
    m_rotationSliderY->setTickInterval(10);
    m_rotationSliderY->setMinimum(0);
    m_rotationSliderY->setValue(m_modifier->originalYRotation());
    m_rotationSliderY->setMaximum(90);
    m_zoomSlider = new QSlider(Qt::Horizontal, m_widget);
    m_zoomSlider->setTickInterval(10);
    m_zoomSlider->setMinimum(0);
    m_zoomSlider->setValue(m_modifier->originalZoomLevel());
    m_zoomSlider->setMaximum(600);

    m_resetCameraButton = new QPushButton(m_widget);
    m_resetCameraButton->setText(tr("Reset view"));
    viewVBox->addWidget(m_resetCameraButton);

    m_cameraPOVButton = new QPushButton(m_widget);
    viewVBox->addWidget(m_cameraPOVButton);

    QGroupBox *rotateXGroupBox = new QGroupBox(tr("Rotate horizontally"));
    QHBoxLayout *rotateXHBox = new QHBoxLayout();
    rotateXHBox->addWidget(m_rotationSliderX);
    rotateXGroupBox->setLayout(rotateXHBox);
    viewVBox->addWidget(rotateXGroupBox);

    QGroupBox *rotateYGroupBox = new QGroupBox(tr("Rotate vertically"));
    QHBoxLayout *rotateYHBox = new QHBoxLayout();
    rotateYHBox->addWidget(m_rotationSliderY);
    rotateYGroupBox->setLayout(rotateYHBox);
    viewVBox->addWidget(rotateYGroupBox);

    QGroupBox *zoomGroupBox = new QGroupBox(tr("Zoom"));
    QHBoxLayout *zoomHBox = new QHBoxLayout();
    zoomHBox->addWidget(m_zoomSlider);
    zoomGroupBox->setLayout(zoomHBox);
    viewVBox->addWidget(zoomGroupBox);

    m_axisMinSliderX = new QSlider(Qt::Horizontal, m_widget);
    m_axisMinSliderX->setMinimum(0);
    m_axisMinSliderX->setTickInterval(1);
    m_axisMaxSliderX = new QSlider(Qt::Horizontal, m_widget);
    m_axisMaxSliderX->setMinimum(1);
    m_axisMaxSliderX->setTickInterval(1);
    m_axisMinSliderZ = new QSlider(Qt::Horizontal, m_widget);
    m_axisMinSliderZ->setMinimum(0);
    m_axisMinSliderZ->setTickInterval(1);
    m_axisMaxSliderZ = new QSlider(Qt::Horizontal, m_widget);
    m_axisMaxSliderZ->setMinimum(1);
    m_axisMaxSliderZ->setTickInterval(1);

    QGroupBox *axisXRangeGroupBox = new QGroupBox(tr("Column range (X slider)"));
    QVBoxLayout *axisXRangeVBox = new QVBoxLayout();
    axisXRangeVBox->addWidget(m_axisMinSliderX);
    axisXRangeVBox->addWidget(m_axisMaxSliderX);
    axisXRangeGroupBox->setLayout(axisXRangeVBox);
    viewVBox->addWidget(axisXRangeGroupBox);

    QGroupBox *axisZRangeGroupBox = new QGroupBox(tr("Row range (Z slider)"));
    QVBoxLayout *axisZRangeVBox = new QVBoxLayout();
    axisZRangeVBox->addWidget(m_axisMinSliderZ);
    axisZRangeVBox->addWidget(m_axisMaxSliderZ);
    axisZRangeGroupBox->setLayout(axisZRangeVBox);
    viewVBox->addWidget(axisZRangeGroupBox);

    viewGroupBox->setLayout(viewVBox);
    scrollLayout->addWidget(viewGroupBox);
    // -----

    // -----
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
    map<int, QString> m_intToColormap;
    m_intToColormap[0] = "Default";
    m_intToColormap[(int)ColormapType::Heat] = "Heat";
    m_intToColormap[(int)ColormapType::Jet] = "Jet";
    m_intToColormap[(int)ColormapType::Hot] = "Hot";
    m_intToColormap[(int)ColormapType::Gray] = "Gray";
    m_intToColormap[(int)ColormapType::Magma] = "Magma";
    m_intToColormap[(int)ColormapType::Inferno] = "Inferno";
    m_intToColormap[(int)ColormapType::Plasma] = "Plasma";
    m_intToColormap[(int)ColormapType::Viridis] = "Viridis";
    m_intToColormap[(int)ColormapType::Cividis] = "Cividis";
    m_intToColormap[(int)ColormapType::Github] = "Github";

    const int pixMapWidth = 150;
    const int pixMapHeight = 15;
    QPixmap pm(pixMapWidth, pixMapHeight);
    QPainter pmp(&pm);
    pmp.setPen(Qt::NoPen);
    QLinearGradient gr(0, 0, pixMapWidth, pixMapHeight);

    m_colormapList = new QComboBox();
    map<int, QString>::iterator it;
    for(it = m_intToColormap.begin(); it != m_intToColormap.end(); ++it) {
        if (it->second == "Default") {
            gr.setColorAt(0, QColor(234, 234, 234));
        }
        else {
            for (float i = 0; i <= 1 ; i += 0.1) {
                const QColor color = tinycolormap::GetColor(i, (ColormapType)it->first).ConvertToQColor();
                gr.setColorAt(i, color);
            }
        }
        pmp.setBrush(QBrush(gr));
        pmp.drawRect(0, 0, pixMapWidth, pixMapHeight);
        m_colormapList->addItem(QIcon(pm), it->second);
        m_intToLinearGradient[it->first] = gr;
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

    scrollLayout->addWidget(themeGroupBox);
    // -----

    scrollArea->setWidget(scrollWidget);
    rightVLayout->addWidget(scrollArea);

    QObject::connect(m_costFunctionLineEdit, &QLineEdit::textEdited,
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
                     m_modifier, SLOT(setTheme(int)));
    QObject::connect(m_rotationSliderX, &QSlider::valueChanged, m_modifier, &SurfaceGraph::rotateX);
    QObject::connect(m_rotationSliderY, &QSlider::valueChanged, m_modifier, &SurfaceGraph::rotateY);
    QObject::connect(m_zoomSlider, &QSlider::valueChanged, m_modifier, &SurfaceGraph::zoom);
    QObject::connect(m_resetCameraButton, SIGNAL(clicked()), this,
                     SLOT(resetCamera()));
    QObject::connect(m_cameraPOVButton, &QPushButton::pressed, m_modifier,
                     &SurfaceGraph::setCameraPreset);
    QObject::connect(m_computePartialDerivativesButton, &QPushButton::pressed,
                     m_modifier, &SurfaceGraph::computePartialDerivatives);
    QObject::connect(m_costFunctionList, SIGNAL(currentIndexChanged(int)),
                     m_modifier, SLOT(setCostFunction(int)));
    QObject::connect(m_viewList, SIGNAL(currentIndexChanged(int)),
                     m_modifier, SLOT(setSelectionMode(int)));
    QObject::connect(m_colormapList, SIGNAL(activated(int)),
                     m_modifier, SLOT(setColormap(int)));
    QObject::connect(m_surfaceList, SIGNAL(currentIndexChanged(int)),
                     m_modifier, SLOT(setSurface(int)));

    QObject::connect(m_cameraPOVButton, SIGNAL(clicked()), this,
                     SLOT(updateSliders()));

    QObject::connect(m_graph->scene()->activeCamera(), SIGNAL(xRotationChanged(float)), this,
                     SLOT(updateXRotationSlider(float)));
    QObject::connect(m_graph->scene()->activeCamera(), SIGNAL(yRotationChanged(float)), this,
                     SLOT(updateYRotationSlider(float)));
    QObject::connect(m_graph->scene()->activeCamera(), SIGNAL(zoomLevelChanged(float)), this,
                     SLOT(updateZoomLevelSlider(float)));

    QObject::connect(m_modifier->series(), SIGNAL(selectedPointChanged(QPoint)), this,
    SLOT(setSelectedPoint(QPoint)));
    QObject::connect(m_runGradientDescentButton, SIGNAL(clicked()), this,
                     SLOT(runGradientDescent()));

    m_modifier->setAxisMinSliderX(m_axisMinSliderX);
    m_modifier->setAxisMaxSliderX(m_axisMaxSliderX);
    m_modifier->setAxisMinSliderZ(m_axisMinSliderZ);
    m_modifier->setAxisMaxSliderZ(m_axisMaxSliderZ);

    m_costFunctionList->setCurrentIndex(1);
    m_themeList->activated(2);
    m_themeList->setCurrentIndex(2);
    m_colormapList->activated(0);
    m_surfaceList->setCurrentIndex(2);
    m_viewList->setCurrentIndex(0);

    m_modifier->setCameraPreset();
}

void MainWindow::initializeLeftVLayout(QVBoxLayout *leftVLayout)
{
    QScrollArea *scrollArea = 0;
    scrollArea = initializeScrollArea(scrollArea);

    // Create a widget that will contain other widgets and layouts
    QWidget *scrollWidget = new QWidget(scrollArea);
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);

    // -----
    m_resultButton = new QPushButton(tr("Show results"), m_widget);
    scrollLayout->addWidget(m_resultButton);
    QObject::connect(m_resultButton, SIGNAL(clicked()),
                     this, SLOT(openResultWindow()));
    // -----
    m_toggleCurvesButton = new QPushButton(tr("Toggle curves"), m_widget);
    m_toggleCurvesButton->setCheckable(true);
    scrollLayout->addWidget(m_toggleCurvesButton);
    QObject::connect(m_toggleCurvesButton, SIGNAL(clicked(bool)),
                     this, SLOT(toggleCurves(bool)));
    // -----
    m_toggleCheckboxesButton = new QPushButton(tr("Toggle checkboxes"), m_widget);
    m_toggleCheckboxesButton->setCheckable(true);
    scrollLayout->addWidget(m_toggleCheckboxesButton);
    QObject::connect(m_toggleCheckboxesButton, SIGNAL(clicked(bool)),
                     this, SLOT(toggleCheckboxes(bool)));
    // -----
    m_resetValuesButton = new QPushButton(tr("Reset values"), m_widget);
    scrollLayout->addWidget(m_resetValuesButton);
    QObject::connect(m_resetValuesButton, SIGNAL(pressed()),
                     this, SLOT(resetValues()));
    // -----

    const int width = 20;
    const int height = 2;
    QPixmap pm(width, height);
    QPainter pmp(&pm);
    pmp.setPen(Qt::NoPen);

    map<QString, GradientDescent*>::iterator it;
    for (it = m_gradientDescentMethodToGradientDescent.begin(); it != m_gradientDescentMethodToGradientDescent.end(); ++it) {

        // -----
        QFormLayout *formLayout = new QFormLayout();
        QCheckBox *runGDAlgortihmsCheckbox = new QCheckBox();
        runGDAlgortihmsCheckbox->setChecked(true);
        m_gradientDescentMethodToCheckBox[it->first] = runGDAlgortihmsCheckbox;

        pmp.setBrush(QBrush(it->second->color()));
        pmp.drawRect(0, 0, width, height);
        QPushButton *button = new QPushButton(QIcon(pm), it->second->name(), m_widget);
        button->setIconSize(QSize(width, height));
        button->setStyleSheet("Text-align: left; padding: 4px 0px 4px 4px;");
        button->setProperty("id", m_gradientDescentMethodToInt[it->first]);
        button->setCheckable(true);
        QObject::connect(button, SIGNAL(toggled(bool)),
                         this, SLOT(toggleCurve(bool)));
        m_gradientDescentMethodToPushButton[it->first] = button;

        formLayout->addRow(runGDAlgortihmsCheckbox, button);

        scrollLayout->addLayout(formLayout);
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
           QDoubleSpinBox *spinBox = new QDoubleSpinBox(m_widget);
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
           spinBox->setValue(it->second->hyperParameterDefaultValue(hyperParameter));

           gridLayout->addWidget(spinBox, i, j);
           j++;
           m_keyToSpinBox[key(it->first, hyperParameter)] = spinBox;

           gridLayout->addWidget(new QLabel(hyperParameter, m_widget), i, j);
           j++;
           hyperParameterVLayout->addLayout(gridLayout);
        }

        // -----
        scrollLayout->addLayout(hyperParameterVLayout);
    }
    scrollArea->setWidget(scrollWidget);
    leftVLayout->addWidget(scrollArea);
}

void MainWindow::toggleCurves(bool checked)
{
    map<QString, QPushButton*>::iterator it;
    for (it = m_gradientDescentMethodToPushButton.begin(); it != m_gradientDescentMethodToPushButton.end(); ++it) {
        QPushButton *button = it->second;
        button->setChecked(checked);
    }
}

void MainWindow::resetValues()
{
    map<QString, QDoubleSpinBox*>::iterator it;
    for (it = m_keyToSpinBox.begin(); it != m_keyToSpinBox.end(); ++it) {
        QString gradientDescentMethod = it->first.split("_")[0];
        QString hyperParameter = it->first.split("_")[1];
        QDoubleSpinBox *spinBox = it->second;        
        GradientDescent *gradientDescent = m_gradientDescentMethodToGradientDescent[gradientDescentMethod];
        spinBox->setValue(gradientDescent->hyperParameterDefaultValue(hyperParameter));
    }
}

void MainWindow::toggleCheckboxes(bool checked)
{
    map<QString, QCheckBox*>::iterator it;
    for (it = m_gradientDescentMethodToCheckBox.begin(); it != m_gradientDescentMethodToCheckBox.end(); ++it) {
        it->second->setChecked(!checked);
    }
}

map<int, QLinearGradient> MainWindow::intToLinearGradient() {
    return m_intToLinearGradient;
}

MainWindow::~MainWindow()
{
    delete m_graph;
    delete m_modifier;

    map<QString, GradientDescent*>::iterator it;
    for (it = m_gradientDescentMethodToGradientDescent.begin(); it != m_gradientDescentMethodToGradientDescent.end(); ++it) {
        delete it->second;
    }
}

void MainWindow::openResultWindow()
{
    new ResultWindow(this, m_widget);
}

map<QString, QCheckBox*> MainWindow::gradientDescentMethodToCheckBox()
{
    return m_gradientDescentMethodToCheckBox;
}

map<QString, GradientDescent*> MainWindow::gradientDescentMethodToGradientDescent()
{
    return m_gradientDescentMethodToGradientDescent;
}
