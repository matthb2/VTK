/*=========================================================================

  Program:   Visualization Toolkit
  Module:    TestLinePlot.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "vtkAxis.h"
#include "vtkContextActor.h"
#include "vtkChartXY.h"
#include "vtkColorTransferFunction.h"
#include "vtkColorTransferFunctionItem.h"
#include "vtkCompositeTransferFunctionItem.h"
#include "vtkContextScene.h"
#include "vtkContextView.h"
#include "vtkFloatArray.h"
#include "vtkLookupTable.h"
#include "vtkLookupTableItem.h"
#include "vtkOpenGLExtensionManager.h"
#include "vtkOpenGLRenderWindow.h"
#include "vtkPiecewiseControlPointsItem.h"
#include "vtkPiecewiseFunction.h"
#include "vtkPiecewiseFunctionItem.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

//----------------------------------------------------------------------------
int TestMultipleScalarsToColors(int , char * [])
{
  VTK_CREATE(vtkRenderWindow, renwin);
  renwin->SetMultiSamples(0);
  renwin->SetSize(800, 640);

  VTK_CREATE(vtkRenderWindowInteractor, iren);
  iren->SetRenderWindow(renwin);

  //setup the 4charts view ports
  double viewports[16] ={
    0.0,0.0,0.3,0.5,
    0.3,0.0,1.0,0.5,
    0.0,0.5,0.5,1.0,
    0.5,0.5,1.0,1.0};

  // Lookup Table
  vtkSmartPointer<vtkLookupTable> lookupTable =
    vtkSmartPointer<vtkLookupTable>::New();
  lookupTable->SetAlpha(0.5);
  lookupTable->Build();
  // Color transfer function
  vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction =
    vtkSmartPointer<vtkColorTransferFunction>::New();
  colorTransferFunction->AddHSVSegment(0.,0.,1.,1.,0.3333,0.3333,1.,1.);
  colorTransferFunction->AddHSVSegment(0.3333,0.3333,1.,1.,0.6666,0.6666,1.,1.);
  colorTransferFunction->AddHSVSegment(0.6666,0.6666,1.,1.,1.,0.,1.,1.);
  colorTransferFunction->Build();
  // Opacity function
  vtkSmartPointer<vtkPiecewiseFunction> opacityFunction =
    vtkSmartPointer<vtkPiecewiseFunction>::New();
  opacityFunction->AddPoint(0.,0.);
  opacityFunction->AddPoint(1.,1.);
  for ( int i=0; i < 4; ++i)
    {
    VTK_CREATE(vtkRenderer, ren);
    ren->SetBackground(1.0,1.0,1.0);
    ren->SetViewport(&viewports[i*4]);
    renwin->AddRenderer(ren);

    VTK_CREATE(vtkChartXY, chart);
    VTK_CREATE(vtkContextScene, chartScene);
    VTK_CREATE(vtkContextActor, chartActor);

    chartScene->AddItem(chart);
    chartActor->SetScene(chartScene);

    //both needed
    ren->AddActor(chartActor);
    chartScene->SetRenderer(ren);

    switch (i)
      {
      case 0:
        {
        vtkSmartPointer<vtkLookupTableItem> item =
          vtkSmartPointer<vtkLookupTableItem>::New();
        item->SetLookupTable(lookupTable);
        chart->AddPlot(item);
        chart->SetAutoAxes(false);
        chart->GetAxis(0)->SetVisible(false);
        chart->GetAxis(1)->SetVisible(false);
        chart->SetTitle("vtkLookupTable");
        break;
        }
      case 1:
        {
        vtkSmartPointer<vtkColorTransferFunctionItem> item =
          vtkSmartPointer<vtkColorTransferFunctionItem>::New();
        item->SetColorTransferFunction(colorTransferFunction);
        // opacity is added on the item, not on the transfer function
        item->SetOpacity(0.8);
        chart->AddPlot(item);
        chart->SetTitle("vtkColorTransferFunction");
        break;
        }
      case 2:
        {
        vtkSmartPointer<vtkCompositeTransferFunctionItem> item =
          vtkSmartPointer<vtkCompositeTransferFunctionItem>::New();
        item->SetColorTransferFunction(colorTransferFunction);
        item->SetOpacityFunction(opacityFunction);
        item->SetMaskAboveCurve(true);
        chart->AddPlot(item);
        chart->SetTitle("vtkColorTransferFunction + vtkPiecewiseFunction");
        break;
        }
      case 3:
        {
        vtkSmartPointer<vtkPiecewiseFunctionItem> item =
          vtkSmartPointer<vtkPiecewiseFunctionItem>::New();
        item->SetPiecewiseFunction(opacityFunction);
        item->SetColor(1., 0, 0);
        chart->AddPlot(item);
        vtkSmartPointer<vtkPiecewiseControlPointsItem> controlPointsItem =
          vtkSmartPointer<vtkPiecewiseControlPointsItem>::New();
        controlPointsItem->SetPiecewiseFunction(opacityFunction);
        chart->AddPlot(controlPointsItem);
        chart->SetTitle("vtkPiecewiseFunction");
        break;
        }
      default:
        break;
      }
    }

  iren->Render();
  vtkOpenGLRenderWindow* openGLRenWin =
      vtkOpenGLRenderWindow::SafeDownCast(renwin);
  if (openGLRenWin &&
      openGLRenWin->GetExtensionManager() &&
      openGLRenWin->GetExtensionManager()->ExtensionSupported("GL_VERSION_1_2"))
    {
    // we might be able to support GL Version 1.1 but it requires some modifications
    // on how to apply 1D textures.
    iren->Initialize();
    iren->Start();
    }
  else
    {
    cout << "GL version 1.2 or higher is required." << endl;
    }

  return EXIT_SUCCESS;
}
