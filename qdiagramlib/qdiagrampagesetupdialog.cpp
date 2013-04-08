#include "stdafx.h"
#include "qdiagrampagesetupdialog.h"
#include "ui_qdiagrampagesetupdialog.h"

#include <qdiagram.h>
#include <qdiagramsheet.h>

QDiagramPageSetupDialog::QDiagramPageSetupDialog(QAbstractDiagram* diagram, QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::QDiagramPageSetupDialog();
	ui->setupUi(this);

	m_paperSizes[QDiagramToolkit::A0] = QSizeF(841, 1189);
	m_paperSizes[QDiagramToolkit::A1] = QSizeF(594, 841);
	m_paperSizes[QDiagramToolkit::A2] = QSizeF(420, 594);
	m_paperSizes[QDiagramToolkit::A3] = QSizeF(297, 420);
	m_paperSizes[QDiagramToolkit::A4] = QSizeF(210, 297);

	m_paperSizeNames[QDiagramToolkit::A0] = tr("A0");
	m_paperSizeNames[QDiagramToolkit::A1] = tr("A1");
	m_paperSizeNames[QDiagramToolkit::A2] = tr("A2");
	m_paperSizeNames[QDiagramToolkit::A3] = tr("A3");
	m_paperSizeNames[QDiagramToolkit::A4] = tr("A4");

	m_diagram = diagram;

	m_paperOrientation = m_diagram->currentSheet()->paperOrientation();
	m_paperSize = m_diagram->currentSheet()->paperSize();
	m_paperSizeF = m_diagram->currentSheet()->paperSize(QDiagramToolkit::Millimeter);

	paperSettingsChanged();

	ui->nameLineEdit->setText(m_diagram->currentSheet()->name());

	ui->portraitRadioButton->blockSignals(true);
	if (m_diagram->currentSheet()->paperOrientation() == QDiagramToolkit::Portrait){
		ui->portraitRadioButton->setChecked(true);
	} else {
		ui->landscapeRadioButton->setChecked(true);
	}
	ui->portraitRadioButton->blockSignals(false);

	if (m_paperSize == QDiagramToolkit::Custom){
		ui->userDefinedSizeRadioButton->setChecked(true);
	}
}

QDiagramPageSetupDialog::~QDiagramPageSetupDialog()
{
	delete ui;
}

void QDiagramPageSetupDialog::applyChanges()
{
	if (ui->portraitRadioButton->isChecked()){
		m_diagram->currentSheet()->setPaperOrientation(QDiagramToolkit::Portrait);
	} else {
		m_diagram->currentSheet()->setPaperOrientation(QDiagramToolkit::Landscape);
	}

	if (ui->predefinedSizeRadioButton->isChecked()){
		m_diagram->currentSheet()->setPaperSize(static_cast<QDiagramToolkit::PaperSize>(ui->paperSizeComboBox->itemData(ui->paperSizeComboBox->currentIndex()).toInt()));
	} else if (ui->userDefinedSizeRadioButton->isChecked()){
		m_diagram->currentSheet()->setPaperSize(QSizeF(ui->pageWidthSpinBox->value(), ui->pageHeightSpinBox->value()), QDiagramToolkit::Millimeter);
	}
	m_diagram->currentSheet()->setName(ui->nameLineEdit->text());
}

void QDiagramPageSetupDialog::buttonBoxClicked(QAbstractButton* button)
{
	if (button == ui->buttonBox->button(QDialogButtonBox::Apply)){
		applyChanges();
	} else if (button == ui->buttonBox->button(QDialogButtonBox::Cancel)){
		reject();
	} else if (button == ui->buttonBox->button(QDialogButtonBox::Ok)){
		applyChanges();
		accept();
	}
}

void QDiagramPageSetupDialog::paperSettingsChanged()
{
	ui->paperSizeComboBox->clear();
	QMapIterator<QDiagramToolkit::PaperSize, QSizeF> it(m_paperSizes);
	while(it.hasNext()){
		it.next();
		if (m_paperOrientation == QDiagramToolkit::Portrait){
			ui->paperSizeComboBox->addItem(QString("%1: %2 mm x %3 mm").arg(m_paperSizeNames.value(it.key())).arg(it.value().width()).arg(it.value().height()), it.key());
		} else {
			ui->paperSizeComboBox->addItem(QString("%1: %2 mm x %3 mm").arg(m_paperSizeNames.value(it.key())).arg(it.value().height()).arg(it.value().width()), it.key());
		}
	}
	if (m_paperSize != QDiagramToolkit::Custom){
		ui->paperSizeComboBox->setCurrentIndex(ui->paperSizeComboBox->findData(m_paperSize));
	}
	ui->pageWidthSpinBox->setValue(m_paperSizeF.width());
	ui->pageHeightSpinBox->setValue(m_paperSizeF.height());
}

void QDiagramPageSetupDialog::paperSizeComboBoxActivated(int index)
{
	m_paperSize = static_cast<QDiagramToolkit::PaperSize>(ui->paperSizeComboBox->itemData(index).toInt());
	if (m_paperOrientation == QDiagramToolkit::Portrait){
		m_paperSizeF.setWidth(m_paperSizes.value(static_cast<QDiagramToolkit::PaperSize>(ui->paperSizeComboBox->itemData(index).toInt())).width());
		m_paperSizeF.setHeight(m_paperSizes.value(static_cast<QDiagramToolkit::PaperSize>(ui->paperSizeComboBox->itemData(index).toInt())).height());
	} else {
		m_paperSizeF.setWidth(m_paperSizes.value(static_cast<QDiagramToolkit::PaperSize>(ui->paperSizeComboBox->itemData(index).toInt())).height());
		m_paperSizeF.setHeight(m_paperSizes.value(static_cast<QDiagramToolkit::PaperSize>(ui->paperSizeComboBox->itemData(index).toInt())).width());
	}
	paperSettingsChanged();
}

void QDiagramPageSetupDialog::portraitRadioButtonToggled(bool on)
{
	if (on){
		m_paperOrientation = QDiagramToolkit::Portrait;
	} else {
		m_paperOrientation = QDiagramToolkit::Landscape;
	}
	QSizeF s;
	s.setHeight(m_paperSizeF.width());
	s.setWidth(m_paperSizeF.height());
	m_paperSizeF = s;
	paperSettingsChanged();
}

void QDiagramPageSetupDialog::predefinedSizeRadioButtonToggled(bool on)
{
	if (on){
		ui->paperSizeComboBox->setEnabled(true);
		ui->pageWidthSpinBox->setEnabled(false);
		ui->pageHeightSpinBox->setEnabled(false);
		paperSizeComboBoxActivated(ui->paperSizeComboBox->currentIndex());
	}
}

void QDiagramPageSetupDialog::userDefinedSizeRadioButtonToggled(bool on)
{
	if (on){
		ui->paperSizeComboBox->setEnabled(false);
		ui->pageWidthSpinBox->setEnabled(true);
		ui->pageHeightSpinBox->setEnabled(true);
	}
}
