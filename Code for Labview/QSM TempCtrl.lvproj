<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="15008000">
	<Property Name="NI.LV.All.SourceOnly" Type="Bool">false</Property>
	<Item Name="My Computer" Type="My Computer">
		<Property Name="IOScan.Faults" Type="Str"></Property>
		<Property Name="IOScan.NetVarPeriod" Type="UInt">100</Property>
		<Property Name="IOScan.NetWatchdogEnabled" Type="Bool">false</Property>
		<Property Name="IOScan.Period" Type="UInt">10000</Property>
		<Property Name="IOScan.PowerupMode" Type="UInt">0</Property>
		<Property Name="IOScan.Priority" Type="UInt">9</Property>
		<Property Name="IOScan.ReportModeConflict" Type="Bool">true</Property>
		<Property Name="IOScan.StartEngineOnDeploy" Type="Bool">false</Property>
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="Project Documentation" Type="Folder">
			<Property Name="NI.SortType" Type="Int">3</Property>
			<Item Name="Documentation Images" Type="Folder">
				<Property Name="NI.SortType" Type="Int">0</Property>
				<Item Name="loc_access_task_data.png" Type="Document" URL="../documentation/loc_access_task_data.png"/>
				<Item Name="loc_bundle_new_button_ref.png" Type="Document" URL="../documentation/loc_bundle_new_button_ref.png"/>
				<Item Name="loc_convert_variant.png" Type="Document" URL="../documentation/loc_convert_variant.png"/>
				<Item Name="loc_create_two_queues.png" Type="Document" URL="../documentation/loc_create_two_queues.png"/>
				<Item Name="loc_dequeue_error_case.png" Type="Document" URL="../documentation/loc_dequeue_error_case.png"/>
				<Item Name="loc_disable_new_button.png" Type="Document" URL="../documentation/loc_disable_new_button.png"/>
				<Item Name="loc_enqueue_generic_message.png" Type="Document" URL="../documentation/loc_enqueue_generic_message.png"/>
				<Item Name="loc_enqueue_message_with_data.png" Type="Document" URL="../documentation/loc_enqueue_message_with_data.png"/>
				<Item Name="loc_enqueue_new_message.png" Type="Document" URL="../documentation/loc_enqueue_new_message.png"/>
				<Item Name="loc_enqueue_priority_message.png" Type="Document" URL="../documentation/loc_enqueue_priority_message.png"/>
				<Item Name="loc_exit_message.png" Type="Document" URL="../documentation/loc_exit_message.png"/>
				<Item Name="loc_message_queue_wire.png" Type="Document" URL="../documentation/loc_message_queue_wire.png"/>
				<Item Name="loc_new_message_diagram.png" Type="Document" URL="../documentation/loc_new_message_diagram.png"/>
				<Item Name="loc_new_task_loop.png" Type="Document" URL="../documentation/loc_new_task_loop.png"/>
				<Item Name="loc_new_task_typedef.png" Type="Document" URL="../documentation/loc_new_task_typedef.png"/>
				<Item Name="loc_open_msg_queue_typedef.png" Type="Document" URL="../documentation/loc_open_msg_queue_typedef.png"/>
				<Item Name="loc_qmh_ignore_errors.png" Type="Document" URL="../documentation/loc_qmh_ignore_errors.png"/>
				<Item Name="loc_queued_message_handler.gif" Type="Document" URL="../documentation/loc_queued_message_handler.gif"/>
				<Item Name="loc_stop_new_mhl.png" Type="Document" URL="../documentation/loc_stop_new_mhl.png"/>
				<Item Name="loc_stop_task.png" Type="Document" URL="../documentation/loc_stop_task.png"/>
				<Item Name="loc_ui_data.png" Type="Document" URL="../documentation/loc_ui_data.png"/>
				<Item Name="loc_value_change_event.png" Type="Document" URL="../documentation/loc_value_change_event.png"/>
				<Item Name="noloc_note.png" Type="Document" URL="../documentation/noloc_note.png"/>
				<Item Name="noloc_tip.png" Type="Document" URL="../documentation/noloc_tip.png"/>
			</Item>
			<Item Name="Queued Message Handler Documentation.html" Type="Document" URL="../documentation/Queued Message Handler Documentation.html"/>
		</Item>
		<Item Name="QSM support VIs" Type="Folder">
			<Property Name="NI.SortType" Type="Int">3</Property>
			<Item Name="Message Queue.lvlib" Type="Library" URL="../support/Message Queue/Message Queue.lvlib"/>
			<Item Name="User Event - Stop.lvlib" Type="Library" URL="../support/User Event - Stop/User Event - Stop.lvlib"/>
			<Item Name="Check Loop Error.vi" Type="VI" URL="../support/Check Loop Error.vi"/>
			<Item Name="Error Handler - Message Handling Loop.vi" Type="VI" URL="../support/Error Handler - Message Handling Loop.vi"/>
			<Item Name="Error Handler - Event Handling Loop.vi" Type="VI" URL="../support/Error Handler - Event Handling Loop.vi"/>
		</Item>
		<Item Name="SubVIs" Type="Folder">
			<Item Name="Conversion" Type="Folder">
				<Item Name="ConvertADCReading.vi" Type="VI" URL="../SubVIs/Conversion/ConvertADCReading.vi"/>
				<Item Name="ConvertToADCReading.vi" Type="VI" URL="../SubVIs/Conversion/ConvertToADCReading.vi"/>
				<Item Name="CtrlToVolts.vi" Type="VI" URL="../SubVIs/Conversion/CtrlToVolts.vi"/>
				<Item Name="ErrorToResistance.vi" Type="VI" URL="../SubVIs/Conversion/ErrorToResistance.vi"/>
				<Item Name="ErrorToTemperature.vi" Type="VI" URL="../SubVIs/Conversion/ErrorToTemperature.vi"/>
				<Item Name="ErrorToVolts.vi" Type="VI" URL="../SubVIs/Conversion/ErrorToVolts.vi"/>
				<Item Name="ResistanceToError.vi" Type="VI" URL="../SubVIs/Conversion/ResistanceToError.vi"/>
				<Item Name="ResistanceToTemperature.vi" Type="VI" URL="../SubVIs/Conversion/ResistanceToTemperature.vi"/>
				<Item Name="TemperatureToError.vi" Type="VI" URL="../SubVIs/Conversion/TemperatureToError.vi"/>
				<Item Name="TemperatureToResistance.vi" Type="VI" URL="../SubVIs/Conversion/TemperatureToResistance.vi"/>
				<Item Name="VoltsToCtrl.vi" Type="VI" URL="../SubVIs/Conversion/VoltsToCtrl.vi"/>
				<Item Name="VoltsToError.vi" Type="VI" URL="../SubVIs/Conversion/VoltsToError.vi"/>
				<Item Name="VoltsToResistance.vi" Type="VI" URL="../SubVIs/Conversion/VoltsToResistance.vi"/>
			</Item>
			<Item Name="GenerateHeaders.vi" Type="VI" URL="../SubVIs/GenerateHeaders.vi"/>
			<Item Name="GenFilenames.vi" Type="VI" URL="../SubVIs/GenFilenames.vi"/>
			<Item Name="getEnumValues.vi" Type="VI" URL="../SubVIs/getEnumValues.vi"/>
			<Item Name="LoopAndQueryInput.vi" Type="VI" URL="../SubVIs/LoopAndQueryInput.vi"/>
			<Item Name="LoopAndQueryOutput.vi" Type="VI" URL="../SubVIs/LoopAndQueryOutput.vi"/>
			<Item Name="OpenFile.vi" Type="VI" URL="../SubVIs/OpenFile.vi"/>
			<Item Name="PopulateChannelArray.vi" Type="VI" URL="../SubVIs/PopulateChannelArray.vi"/>
			<Item Name="PopulateChannelSettingsArray.vi" Type="VI" URL="../SubVIs/PopulateChannelSettingsArray.vi"/>
			<Item Name="Show ALL Front Panel Objects.vi" Type="VI" URL="../SubVIs/Show ALL Front Panel Objects.vi"/>
			<Item Name="TranslateVoltages.vi" Type="VI" URL="../SubVIs/Conversion/TranslateVoltages.vi"/>
			<Item Name="WriteData.vi" Type="VI" URL="../SubVIs/WriteData.vi"/>
		</Item>
		<Item Name="Type Definitions" Type="Folder">
			<Item Name="BridgeSettings.ctl" Type="VI" URL="../controls/BridgeSettings.ctl"/>
			<Item Name="ChannelSelection.ctl" Type="VI" URL="../controls/ChannelSelection.ctl"/>
			<Item Name="ChannelSettings.ctl" Type="VI" URL="../controls/ChannelSettings.ctl"/>
			<Item Name="UI Data.ctl" Type="VI" URL="../controls/UI Data.ctl"/>
		</Item>
		<Item Name="icon.ico" Type="Document" URL="../icon.ico"/>
		<Item Name="TempCtrlMonitor.vi" Type="VI" URL="../TempCtrlMonitor.vi"/>
		<Item Name="YbTempCtrl.lvlib" Type="Library" URL="../Driver/YbTempCtrl.lvlib"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="BuildHelpPath.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/BuildHelpPath.vi"/>
				<Item Name="Check Special Tags.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Check Special Tags.vi"/>
				<Item Name="Clear Errors.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Clear Errors.vi"/>
				<Item Name="Convert property node font to graphics font.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Convert property node font to graphics font.vi"/>
				<Item Name="Details Display Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Details Display Dialog.vi"/>
				<Item Name="DialogType.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/DialogType.ctl"/>
				<Item Name="DialogTypeEnum.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/DialogTypeEnum.ctl"/>
				<Item Name="Error Cluster From Error Code.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Cluster From Error Code.vi"/>
				<Item Name="Error Code Database.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Code Database.vi"/>
				<Item Name="ErrWarn.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/ErrWarn.ctl"/>
				<Item Name="eventvkey.ctl" Type="VI" URL="/&lt;vilib&gt;/event_ctls.llb/eventvkey.ctl"/>
				<Item Name="Find Tag.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Find Tag.vi"/>
				<Item Name="Format Message String.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Format Message String.vi"/>
				<Item Name="General Error Handler Core CORE.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/General Error Handler Core CORE.vi"/>
				<Item Name="General Error Handler.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/General Error Handler.vi"/>
				<Item Name="Get String Text Bounds.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Get String Text Bounds.vi"/>
				<Item Name="Get Text Rect.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Get Text Rect.vi"/>
				<Item Name="GetHelpDir.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/GetHelpDir.vi"/>
				<Item Name="GetRTHostConnectedProp.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/GetRTHostConnectedProp.vi"/>
				<Item Name="Longest Line Length in Pixels.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Longest Line Length in Pixels.vi"/>
				<Item Name="LVBoundsTypeDef.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/miscctls.llb/LVBoundsTypeDef.ctl"/>
				<Item Name="LVRectTypeDef.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/miscctls.llb/LVRectTypeDef.ctl"/>
				<Item Name="Not Found Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Not Found Dialog.vi"/>
				<Item Name="Search and Replace Pattern.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Search and Replace Pattern.vi"/>
				<Item Name="Set Bold Text.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Set Bold Text.vi"/>
				<Item Name="Set String Value.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Set String Value.vi"/>
				<Item Name="Simple Error Handler.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Simple Error Handler.vi"/>
				<Item Name="TagReturnType.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/TagReturnType.ctl"/>
				<Item Name="Three Button Dialog CORE.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Three Button Dialog CORE.vi"/>
				<Item Name="Three Button Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Three Button Dialog.vi"/>
				<Item Name="Trim Whitespace.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Trim Whitespace.vi"/>
				<Item Name="whitespace.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/whitespace.ctl"/>
			</Item>
			<Item Name="Measure-Step-Fn.vi" Type="VI" URL="../Driver/Public/Action-Status/Step-Function/Measure-Step-Fn.vi"/>
			<Item Name="MeasureError.vi" Type="VI" URL="../Driver/Public/Action-Status/MeasureError.vi"/>
			<Item Name="OutputConstCurrent.vi" Type="VI" URL="../Driver/Public/Action-Status/OutputConstCurrent.vi"/>
			<Item Name="Start-Step.vi" Type="VI" URL="../Driver/Public/Action-Status/Step-Function/Start-Step.vi"/>
			<Item Name="StartLockCurrent.vi" Type="VI" URL="../Driver/Public/Action-Status/StartLockCurrent.vi"/>
		</Item>
		<Item Name="Build Specifications" Type="Build">
			<Item Name="TempCtrl" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{962B20A6-330D-40DE-8C4F-44847D9D9B46}</Property>
				<Property Name="App_INI_GUID" Type="Str">{C0292861-5DCC-498D-BE26-98E61823BE6A}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{ABBDBEDA-B674-46A8-BEE3-6A4D4D5F16FC}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">TempCtrl</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../builds</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToProject</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{2399496D-4B7A-4D17-9412-F4F84CB33CE0}</Property>
				<Property Name="Bld_version.build" Type="Int">15</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">TempCtrl.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../builds/TempCtrl.exe</Property>
				<Property Name="Destination[0].path.type" Type="Str">relativeToProject</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../builds/data</Property>
				<Property Name="Destination[1].path.type" Type="Str">relativeToProject</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Exe_iconItemID" Type="Ref">/My Computer/icon.ico</Property>
				<Property Name="Source[0].itemID" Type="Str">{A3DA98C2-11F0-4D09-B5E0-70AF36060751}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/TempCtrlMonitor.vi</Property>
				<Property Name="Source[1].properties[0].type" Type="Str">Run when opened</Property>
				<Property Name="Source[1].properties[0].value" Type="Bool">false</Property>
				<Property Name="Source[1].propertiesCount" Type="Int">1</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">NPL</Property>
				<Property Name="TgtF_fileDescription" Type="Str">Temperature control interface for the arduino PID controller</Property>
				<Property Name="TgtF_internalName" Type="Str">TempCtrl</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2015 </Property>
				<Property Name="TgtF_productName" Type="Str">Arduino Temperature Control</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{0F5EEE7D-922D-480C-926A-5467A4E35DBC}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">TempCtrl.exe</Property>
			</Item>
		</Item>
	</Item>
</Project>
