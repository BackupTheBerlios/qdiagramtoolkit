#ifndef QDIAGRAMPAGESETUPDIALOG_H
#define QDIAGRAMPAGESETUPDIALOG_H

#include "qdiagramlib_global.h"

#include <QDialog>

#include <qdiagramtoolkit.h>

namespace Ui {class QDiagramPageSetupDialog;};

class QAbstractDiagram;

class QDIAGRAMLIBSHARED_EXPORT QDiagramPageSetupDialog : public QDialog
{
	Q_OBJECT
public:
	QDiagramPageSetupDialog(QAbstractDiagram* diagram, QWidget *parent = 0);
	~QDiagramPageSetupDialog();
private slots:
	void buttonBoxClicked(QAbstractButton* button);
	void paperSizeComboBoxActivated(int index);
	void portraitRadioButtonToggled(bool on);
	void predefinedSizeRadioButtonToggled(bool on);
	void userDefinedSizeRadioButtonToggled(bool on);
private:
	void applyChanges();
	void paperSettingsChanged();

	QAbstractDiagram* m_diagram;
	QDiagramToolkit::PaperOrientation m_paperOrientation;
	QDiagramToolkit::PaperSize m_paperSize;
	QSizeF m_paperSizeF;
	QMap<QDiagramToolkit::PaperSize, QSizeF> m_paperSizes;
	QMap<QDiagramToolkit::PaperSize, QString> m_paperSizeNames;

	Ui::QDiagramPageSetupDialog *ui;
};

#endif // QDIAGRAMPAGESETUPDIALOG_H
