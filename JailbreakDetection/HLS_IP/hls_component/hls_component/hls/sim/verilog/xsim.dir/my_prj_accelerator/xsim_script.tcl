set_param project.enableReportConfiguration 0
load_feature core
current_fileset
xsim {my_prj_accelerator} -view {{my_prj_accelerator_dataflow_ana.wcfg}} -tclbatch {my_prj_accelerator.tcl} -protoinst {my_prj_accelerator.protoinst}
