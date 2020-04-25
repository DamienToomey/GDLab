#include "mainwindow.h"
#include "vanillagradientdescent.h"

MainWindow::MainWindow(Q3DSurface *graph, QWidget *container)
{
    m_graph = graph;
    m_container = container;
    m_modifier = new SurfaceGraph(m_graph, this);

    m_widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(m_widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    hLayout->addWidget(m_container, 1);
    hLayout->addLayout(vLayout);
    vLayout->setAlignment(Qt::AlignTop);

    m_widget->setWindowTitle(QStringLiteral("Gradient Descent Visualization"));

    m_functionList = new QComboBox(m_widget);
    m_functionList->addItem(QStringLiteral("Inclined Taco Shell"));
    m_functionList->addItem(QStringLiteral("Sqrt && Sin"));
    m_functionList->addItem(QStringLiteral("Saddle"));
    m_functionList->addItem(QStringLiteral("NonConvex"));

    QLabel *fLabel = new QLabel("y = f(x, z) =");
    m_fLineEdit = new QLineEdit(m_widget);
    m_fLineEdit->setPlaceholderText(QStringLiteral(""));
    m_fLineEdit->setEnabled(true);

    QLabel *dfdxLabel = new QLabel("df/dx =");
    m_dfdxLineEdit = new QLineEdit(m_widget);
    m_dfdxLineEdit->setPlaceholderText(QStringLiteral(""));
    m_dfdxLineEdit->setEnabled(true);

    QLabel *dfdzLabel = new QLabel("df/dz =");
    m_dfdzLineEdit = new QLineEdit(m_widget);
    m_dfdzLineEdit->setPlaceholderText(QStringLiteral(""));
    m_dfdzLineEdit->setEnabled(true);

    m_computePartialDerivativesButton = new QPushButton(m_widget);
    m_computePartialDerivativesButton->setText("Compute partial derivatives");

    QGroupBox *functionGroupBox = new QGroupBox(QStringLiteral("Function"));
    QVBoxLayout *functionLayout = new QVBoxLayout;
    functionLayout->addWidget(m_functionList);
    QHBoxLayout *fLayout = new QHBoxLayout;
    fLayout->addWidget(fLabel);
    fLayout->addWidget(m_fLineEdit);
    functionLayout->addLayout(fLayout);
    functionLayout->addWidget(m_computePartialDerivativesButton);
    QHBoxLayout *dfdxLayout = new QHBoxLayout;
    dfdxLayout->addWidget(dfdxLabel);
    dfdxLayout->addWidget(m_dfdxLineEdit);
    functionLayout->addLayout(dfdxLayout);
    QHBoxLayout *dfdzLayout = new QHBoxLayout;
    dfdzLayout->addWidget(dfdzLabel);
    dfdzLayout->addWidget(m_dfdzLineEdit);
    functionLayout->addLayout(dfdzLayout);
    functionGroupBox->setLayout(functionLayout);

    m_axisMinSliderX = new QSlider(Qt::Horizontal, m_widget);
    m_axisMinSliderX->setMinimum(0);
    m_axisMinSliderX->setTickInterval(1);
    m_axisMinSliderX->setEnabled(true);
    m_axisMaxSliderX = new QSlider(Qt::Horizontal, m_widget);
    m_axisMaxSliderX->setMinimum(1);
    m_axisMaxSliderX->setTickInterval(1);
    m_axisMaxSliderX->setEnabled(true);
    m_axisMinSliderZ = new QSlider(Qt::Horizontal, m_widget);
    m_axisMinSliderZ->setMinimum(0);
    m_axisMinSliderZ->setTickInterval(1);
    m_axisMinSliderZ->setEnabled(true);
    m_axisMaxSliderZ = new QSlider(Qt::Horizontal, m_widget);
    m_axisMaxSliderZ->setMinimum(1);
    m_axisMaxSliderZ->setTickInterval(1);
    m_axisMaxSliderZ->setEnabled(true);

    m_themeList = new QComboBox(m_widget);
    m_themeList->addItem(QStringLiteral("Qt"));
    m_themeList->addItem(QStringLiteral("Primary Colors"));
    m_themeList->addItem(QStringLiteral("Digia"));
    m_themeList->addItem(QStringLiteral("Stone Moss"));
    m_themeList->addItem(QStringLiteral("Army Blue"));
    m_themeList->addItem(QStringLiteral("Retro"));
    m_themeList->addItem(QStringLiteral("Ebony"));
    m_themeList->addItem(QStringLiteral("Isabelle"));

    QGroupBox *themeGroupBox = new QGroupBox(QStringLiteral("Theme"));

    const int pixMapWidth = 250;
    const int pixMapHeight = 15;
    QLinearGradient uniformColorMap(0, 0, pixMapWidth, pixMapHeight);
    uniformColorMap.setColorAt(0.0, Qt::gray);
    QPixmap pm(pixMapWidth, pixMapHeight);
    QPainter pmp(&pm);
    pmp.setBrush(QBrush(uniformColorMap));
    pmp.setPen(Qt::NoPen);
    pmp.drawRect(0, 0, pixMapWidth, pixMapHeight);

    m_colormapList = new QComboBox(m_widget);
    m_colormapList->addItem(QIcon(pm), QStringLiteral("Default"));

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


    m_surfaceList = new QComboBox(m_widget);
    m_surfaceList->addItem(QStringLiteral("WireFrame"));
    m_surfaceList->addItem(QStringLiteral("Surface"));
    m_surfaceList->addItem(QStringLiteral("SurfaceAndWireframe"));

    QVBoxLayout *themeLayout = new QVBoxLayout;
    themeLayout->addWidget(m_themeList);
    themeLayout->addWidget(m_colormapList);
    themeLayout->addWidget(m_surfaceList);
    themeGroupBox->setLayout(themeLayout);

    QGroupBox *axisXRangeHBoxGroupBox = new QGroupBox(QStringLiteral("Column range (X slider)"));
    QHBoxLayout *axisXRangeHBox = new QHBoxLayout;
    axisXRangeHBox->addWidget(m_axisMinSliderX);
    axisXRangeHBox->addWidget(m_axisMaxSliderX);
    axisXRangeHBoxGroupBox->setLayout(axisXRangeHBox);

    QGroupBox *axisZRangeHBoxGroupBox = new QGroupBox(QStringLiteral("Row range (Z slider)"));
    QHBoxLayout *axisZRangeHBox = new QHBoxLayout;
    axisZRangeHBox->addWidget(m_axisMinSliderZ);
    axisZRangeHBox->addWidget(m_axisMaxSliderZ);
    axisZRangeHBoxGroupBox->setLayout(axisZRangeHBox);

    vLayout->addWidget(functionGroupBox);
    vLayout->addWidget(new QLabel(QStringLiteral("Selection Mode")));
    m_selectionModeList = new QComboBox(m_widget);
    m_selectionModeList->addItem(QStringLiteral("Hide Selection"));
    m_selectionModeList->addItem(QStringLiteral("Show Selection"));
    m_selectionModeList->addItem(QStringLiteral("Row Slice"));
    m_selectionModeList->addItem(QStringLiteral("Column Slice"));
    vLayout->addWidget(m_selectionModeList);
    vLayout->addWidget(axisXRangeHBoxGroupBox);
    vLayout->addWidget(axisZRangeHBoxGroupBox);

    vLayout->addWidget(themeGroupBox);

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
    m_resetCameraButton->setText(QStringLiteral("Reset view"));

    m_cameraPOVButton = new QPushButton(m_widget);
    m_cameraPOVButton->setText(QStringLiteral("POV")); // point of view

    QGridLayout *gridLayout = new QGridLayout;

    QVBoxLayout *buttonsVBox = new QVBoxLayout;
    buttonsVBox->addWidget(m_cameraPOVButton, 0, Qt::AlignTop);
    buttonsVBox->addWidget(m_resetCameraButton, 0, Qt::AlignTop);
    gridLayout->addLayout(buttonsVBox, 0, 0);

    QGroupBox *zoomGroupBox = new QGroupBox(QStringLiteral("Zoom"));
    QHBoxLayout *zoomHBox = new QHBoxLayout;
    zoomHBox->addWidget(m_zoomSlider, 0, Qt::AlignTop);
    zoomGroupBox->setLayout(zoomHBox);
    gridLayout->addWidget(zoomGroupBox, 0, 1);

    QGroupBox *rotateXGroupBox = new QGroupBox(QStringLiteral("Rotate horizontally"));
    QHBoxLayout *rotateXHBox = new QHBoxLayout;
    rotateXHBox->addWidget(m_rotationSliderX, 0, Qt::AlignTop);
    rotateXGroupBox->setLayout(rotateXHBox);
    gridLayout->addWidget(rotateXGroupBox, 1, 0);

    QGroupBox *rotateYGroupBox = new QGroupBox(QStringLiteral("Rotate vertically"));
    QHBoxLayout *rotateYHBox = new QHBoxLayout;
    rotateYHBox->addWidget(m_rotationSliderY, 0, Qt::AlignTop);
    rotateYGroupBox->setLayout(rotateYHBox);
    gridLayout->addWidget(rotateYGroupBox, 1, 1);
    vLayout->addLayout(gridLayout);

    m_runGDButton = new QPushButton(m_widget);
    m_runGDButton->setText(QStringLiteral("Run Gradient Descent"));
    vLayout->addWidget(m_runGDButton);

    m_widget->show();

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
    QObject::connect(m_themeList, SIGNAL(currentIndexChanged(int)),
                     m_modifier, SLOT(changeTheme(int)));
    QObject::connect(m_rotationSliderX, &QSlider::valueChanged, m_modifier, &SurfaceGraph::rotateX);
    QObject::connect(m_rotationSliderY, &QSlider::valueChanged, m_modifier, &SurfaceGraph::rotateY);
    QObject::connect(m_zoomSlider, &QSlider::valueChanged, m_modifier, &SurfaceGraph::zoom);
    QObject::connect(m_resetCameraButton, SIGNAL(clicked()), this,
                     SLOT(resetCamera()));
    QObject::connect(m_cameraPOVButton, SIGNAL(clicked()), this,
                     SLOT(updateSliders()));
    QObject::connect(m_computePartialDerivativesButton, &QPushButton::pressed,
                     m_modifier, &SurfaceGraph::computePartialDerivatives);
    QObject::connect(m_functionList, SIGNAL(currentIndexChanged(int)),
                     m_modifier, SLOT(changeFunction(int)));
    QObject::connect(m_selectionModeList, SIGNAL(currentIndexChanged(int)),
                     m_modifier, SLOT(changeSelectionMode(int)));
    QObject::connect(m_colormapList, SIGNAL(currentIndexChanged(int)),
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
                     SLOT(setSelectedPoint(QPoint)));

    QObject::connect(m_runGDButton, SIGNAL(clicked()), this,
                     SLOT(runGradientDescent()));

    m_modifier->setAxisMinSliderX(m_axisMinSliderX);
    m_modifier->setAxisMaxSliderX(m_axisMaxSliderX);
    m_modifier->setAxisMinSliderZ(m_axisMinSliderZ);
    m_modifier->setAxisMaxSliderZ(m_axisMaxSliderZ);

    m_functionList->setCurrentIndex(1);
    m_selectionModeList->setCurrentIndex(1);
    m_themeList->setCurrentIndex(2);
    updateSliders();
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
    m_modifier->changePresetCamera();
    m_rotationSliderX->setValue(m_modifier->xRotation());
    m_rotationSliderY->setValue(m_modifier->yRotation());
    m_cameraPOVButton->setText(QStringLiteral("POV (%1)").arg(m_modifier->cameraPreset()));
    m_modifier->setCameraPreset();
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

void MainWindow::setSelectedPoint(QPoint selectedPoint)
{
    m_selectedPoint = m_modifier->series()->dataProxy()->itemAt(selectedPoint)->position();
}

QVector3D MainWindow::selectedPoint()
{
    return m_selectedPoint;
}

void MainWindow::runGradientDescent()
{
    VanillaGradientDescent *vanillaGradientDescent = new VanillaGradientDescent(this, m_modifier);
    vanillaGradientDescent->run();
}
