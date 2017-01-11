<?xml version='1.0' encoding='UTF-8'?>
<Library LVVersion="15008000">
	<Property Name="Instrument Driver" Type="Str">True</Property>
	<Property Name="NI.Lib.DefaultMenu" Type="Str">dir.mnu</Property>
	<Property Name="NI.Lib.Description" Type="Str">LabVIEW Plug and Play instrument driver for
Arduino based temperature PID controller. </Property>
	<Property Name="NI.Lib.Icon" Type="Bin">&amp;1#!!!!!!!)!"1!&amp;!!!-!%!!!@````]!!!!"!!%!!!)F!!!*Q(C=\&gt;3^D2MR%-8R*]-#H#JX9+C&amp;S2WJ"&lt;5Q,3BVK";G"&lt;7A&amp;FY,;E'B!S@U@[HR1=!:5H)(W-"R&lt;`4RS#6`YO[N..M8[&lt;._07M`(\&lt;\`D('I`\"EK]'P'8`_!PAPH_^8L`O(XL9RE@`2`^`X@_E-?&lt;(UU(PX+46F&amp;+GCEIKZK]&lt;*D?ZS5VO=J-8?:%8?:%8?:%H?:)H?:)H?:)(?:!(?:!(?:$0AVTE)B=ZOW)7.QO:3=U%:D!5G2@D-2\D-2[_SHC-RXC-RXA9)O-R(O-R(O0B."G0]2C0]2A05UW*ZU'/RXC98I7H]"3?QF.Y7&amp;+&amp;JQ!5CR54&amp;Z0!5.&amp;:@#A]B;@Q]&amp;'&amp;J`!5HM*4?/B7Y3E]B;@Q&amp;"Z/G&lt;N35\-=Z(C92IEH]33?R*.YG&amp;K**`%EHM34?&amp;B/C3@R*)BEQ72S#%J/3A9E8R*0YO&amp;.C3@R**\%EXDIGF=IZ]YMGO5ARR.Y!E`A#4S"BSE5?!*0Y!E]A9&gt;J&amp;8A#4_!*0)'(J22Y!E`A#3$"ICSP9,,AR'"1%!1?`O&lt;&gt;%P-KV:4%00J@]X;DKG^!^9WFPG(5.Y,[!KMPH0K#K$&gt;;P9(KD6(`90505106#[MH6!`5F&gt;=,&gt;;:/V*%[5(NK2WWJT8,K'Q_]8K_[8#Y[H]][H5Y[(I][(!\;\`@;\8&lt;;&lt;L@;&lt;$9P4[NP(,?W7JZ,,`H8HP&amp;0@&lt;_^-W$V8H6\,PU,TU:^ULB\T,.(PQ%YO+\0!!!!!!</Property>
	<Property Name="NI.Lib.SourceVersion" Type="Int">352354304</Property>
	<Property Name="NI.Lib.Version" Type="Str">1.0.0.0</Property>
	<Item Name="Examples" Type="Folder">
		<Item Name="ReadError.vi" Type="VI" URL="../Examples/ReadError.vi"/>
	</Item>
	<Item Name="Private" Type="Folder">
		<Property Name="NI.LibItem.Scope" Type="Int">2</Property>
		<Property Name="NI.SortType" Type="Int">3</Property>
		<Item Name="Default Instrument Setup.vi" Type="VI" URL="../Private/Default Instrument Setup.vi"/>
	</Item>
	<Item Name="Public" Type="Folder">
		<Property Name="NI.LibItem.Scope" Type="Int">1</Property>
		<Property Name="NI.SortType" Type="Int">3</Property>
		<Item Name="Action-Status" Type="Folder">
			<Item Name="Status.vi" Type="VI" URL="../Public/Action-Status/Status.vi"/>
			<Item Name="StartLock.vi" Type="VI" URL="../Public/Action-Status/StartLock.vi"/>
		</Item>
		<Item Name="Configure" Type="Folder">
			<Item Name="Set-ConstantOutput.vi" Type="VI" URL="../Public/Action-Status/Set-ConstantOutput.vi"/>
			<Item Name="Set-ControlSignal.vi" Type="VI" URL="../Public/Configure/Set-ControlSignal.vi"/>
			<Item Name="Set-Setpoint.vi" Type="VI" URL="../Public/Configure/Set-Setpoint.vi"/>
			<Item Name="Set-Limits.vi" Type="VI" URL="../Public/Configure/Set-Limits.vi"/>
		</Item>
		<Item Name="Data" Type="Folder">
			<Item Name="Query-ControlSignal.vi" Type="VI" URL="../Public/Data/Query-ControlSignal.vi"/>
			<Item Name="Query-Error.vi" Type="VI" URL="../Public/Data/Query-Error.vi"/>
			<Item Name="Query-Setpoint.vi" Type="VI" URL="../Public/Data/Query-Setpoint.vi"/>
			<Item Name="Query-Limits.vi" Type="VI" URL="../Public/Data/Query-Limits.vi"/>
		</Item>
		<Item Name="Utility" Type="Folder">
			<Item Name="Reset.vi" Type="VI" URL="../Public/Utility/Reset.vi"/>
			<Item Name="Revision Query.vi" Type="VI" URL="../Public/Utility/Revision Query.vi"/>
			<Item Name="Self-Test.vi" Type="VI" URL="../Public/Utility/Self-Test.vi"/>
		</Item>
		<Item Name="dir.mnu" Type="Document" URL="../Public/dir.mnu"/>
		<Item Name="Initialize.vi" Type="VI" URL="../Public/Initialize.vi"/>
		<Item Name="VI Tree.vi" Type="VI" URL="../Public/VI Tree.vi"/>
		<Item Name="Close.vi" Type="VI" URL="../Public/Close.vi"/>
	</Item>
	<Item Name="Typedefs" Type="Folder">
		<Item Name="InputChannelTypedef.ctl" Type="VI" URL="../Typedefs/InputChannelTypedef.ctl"/>
		<Item Name="OutputChannelTypedef.ctl" Type="VI" URL="../Typedefs/OutputChannelTypedef.ctl"/>
	</Item>
</Library>
