#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QLabel>
#include <QPushButton>

SettingsDialog::SettingsDialog(Settings s, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    setModal(true);
    setupLayout();

    onSettingsLoaded(s);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::onSettingsLoaded(Settings newSettings)
{
    s = std::move(newSettings);

    kanjiPerRep->setValue(s.kanjiPerRep);
    maxLevel->setValue(s.maxLevel);
    multiplier->setValue(s.multiplier);
}


void SettingsDialog::onSetDefClicked()
{
    emit settingsSaveRequested(s, true);
}


void SettingsDialog::onSaveClicked()
{
    emit settingsSaveRequested(s, false);
}


void SettingsDialog::onLoadClicked()
{
    emit settingsLoadRequested();
}


void SettingsDialog::setupLayout()
{
    l = new QGridLayout();

    // vertical size should as low as possible
    setFixedSize(width(), 0);

    setupContent();
    setupButtons();

    setLayout(l);

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    adjustSize();
}

void SettingsDialog::setupContent()
{
    QLabel *kanjiRepLabel = new QLabel("Compounds per training cycle");
    kanjiPerRep = new QSpinBox();
    kanjiPerRep->setRange(1, 100);

    connect(kanjiPerRep, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [&](int val){ s.kanjiPerRep = val; });


    QLabel *maxLevelLabel = new QLabel("Maximum level");
    maxLevel = new QSpinBox();
    maxLevel->setRange(1,50);

    connect(maxLevel, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [&](int val){ s.maxLevel = val; });

    QLabel *multiplierLabel = new QLabel("Level multiplier");
    multiplier = new QSpinBox();
    multiplier->setRange(2, 4);

    connect(multiplier, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [&](int val){ s.multiplier = static_cast<int_least8_t>(val); });

    l->addWidget(kanjiRepLabel, 0, 0);
    l->addWidget(kanjiPerRep, 0, 1);

    l->addWidget(maxLevelLabel, 1, 0);
    l->addWidget(maxLevel, 1, 1);

    l->addWidget(multiplierLabel, 2, 0);
    l->addWidget(multiplier, 2, 1);
}

void SettingsDialog::setupButtons()
{
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Save |
                                     QDialogButtonBox::Cancel);

    QPushButton *saveButton = buttonBox->button(QDialogButtonBox::Save);
    saveButton->setText("Save as");

    connect(saveButton, &QPushButton::clicked,
            this, &SettingsDialog::onSaveClicked);
    connect(saveButton, &QPushButton::clicked,
            this, &SettingsDialog::accept);


    QPushButton *setDefButton = new QPushButton("Save to default");
    connect(setDefButton, &QPushButton::clicked,
            this, &SettingsDialog::accept);
    connect(setDefButton, &QPushButton::clicked,
            this, &SettingsDialog::onSetDefClicked);

    buttonBox->addButton(setDefButton, QDialogButtonBox::ButtonRole::AcceptRole);

    QPushButton *loadButton = new QPushButton("Load settings");
    connect(loadButton, &QPushButton::clicked,
            this, &SettingsDialog::onLoadClicked);

    buttonBox->addButton(loadButton, QDialogButtonBox::ButtonRole::ActionRole);

    QPushButton *cancelButton = buttonBox->button(QDialogButtonBox::Cancel);
    connect(cancelButton, &QPushButton::clicked,
            this, &SettingsDialog::reject);

    l->addWidget(buttonBox, 3, 0, 2, 0);
}
