Example 10

This example shows how to use the RNLobby::ProductPatcher class to patch a product.
It also shows how to use WTL with ReplicaNet libraries. The WTL AppWizard will include _ATL_MIN_CRT for release
builds, so remove this define to help stop linker error regarding operator new and delete being multiply defined.
WTL is used to generate a very small minimal application which starts at Example10.cpp _tWinMain().
The maindlg.h class CMainDlg is extended to implement the callbacks from RNReplicaNet::RNLobby::ProductPatcher.
Then maindlg.cpp CMainDlg::OnInitDialog() calls ProductPatcher::Start() to start the product patching process.


This example could be used as a starting point for a live product patcher.

When starting the patcher example assumes the current working directory path is to be the path of the product to patch.

This patcher will then proceed to check a known URL for the patch index and download any product updates if required
to the "Product" directory. The product downloaded is the ReplicaNet demonstration application from the ReplicaNet
website. To test the minimal update capacity of RNLobby::ProductPatcher you may delete or modify the product files
downloaded.

The product is then executed and the patcher exits.

Available project build configurations:
ReleaseMT
DebugMT
