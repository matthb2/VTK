#ifndef __vtkQtDebugLeaksView_h
#define __vtkQtDebugLeaksView_h

#include "QVTKWin32Header.h"
#include <QWidget>

class QModelIndex;
class vtkObjectBase;
class vtkQtDebugLeaksModel;

class QVTK_EXPORT vtkQtDebugLeaksView : public QWidget
{
  Q_OBJECT

public:

  vtkQtDebugLeaksView(QWidget *parent=0);
  virtual ~vtkQtDebugLeaksView();

  vtkQtDebugLeaksModel* model();

  // Description:
  // Returns whether or not the regexp filter is enabled
  bool filterEnabled() const;

  // Description:
  // Returns the regexp filter line edit's current text
  QString filterText() const;

protected:

  virtual void onObjectDoubleClicked(vtkObjectBase* object);
  virtual void onClassNameDoubleClicked(const QString& className);

  // Description:
  // These methods are no-ops when compiled without python
  virtual void addObjectToPython(vtkObjectBase* object);
  virtual void addObjectsToPython(const QList<vtkObjectBase*>& objects);

protected slots:

  void onCurrentRowChanged(const QModelIndex& current);
  void onRowDoubleClicked(const QModelIndex&);
  void onFilterTextChanged(const QString& filterText);
  void onFilterToggled();
  void onFilterHelp();

private:

  class qInternal;
  qInternal* Internal;

  Q_DISABLE_COPY(vtkQtDebugLeaksView);

};

#endif
