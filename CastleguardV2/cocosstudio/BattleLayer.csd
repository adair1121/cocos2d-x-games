<GameFile>
  <PropertyGroup Name="BattleLayer" Type="Layer" ID="fc8fdf3d-aecc-409c-90a7-c5d782792e8d" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" Tag="10" ctype="GameLayerObjectData">
        <Size X="750.0000" Y="1334.0000" />
        <Children>
          <AbstractNodeData Name="bg" ActionTag="667200325" Tag="11" IconVisible="False" ctype="SpriteObjectData">
            <Size X="750.0000" Y="1334.0000" />
            <AnchorPoint ScaleY="1.0000" />
            <Position Y="1334.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition Y="1.0000" />
            <PreSize X="1.0000" Y="1.0000" />
            <FileData Type="Normal" Path="assets/battlebg.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="enemy_tower" ActionTag="114949990" Tag="12" IconVisible="False" BottomMargin="1134.0000" ctype="SpriteObjectData">
            <Size X="750.0000" Y="200.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
            <Position X="375.0000" Y="1334.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="1.0000" />
            <PreSize X="1.0000" Y="0.1499" />
            <FileData Type="Normal" Path="assets/enemy_tower.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="own_tower" ActionTag="1737431232" Tag="13" IconVisible="False" TopMargin="1108.0000" ctype="SpriteObjectData">
            <Size X="750.0000" Y="226.0000" />
            <AnchorPoint ScaleX="0.5000" />
            <Position X="375.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" />
            <PreSize X="1.0000" Y="0.1694" />
            <FileData Type="Normal" Path="assets/own_tower.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="gameCon" ActionTag="1439698516" Tag="14" IconVisible="False" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="750.0000" Y="1334.0000" />
            <Children>
              <AbstractNodeData Name="enemyRect" ActionTag="1839755143" VisibleForFrame="False" Tag="31" IconVisible="False" LeftMargin="0.2394" RightMargin="-0.2394" TopMargin="46.2124" BottomMargin="707.7876" ctype="SpriteObjectData">
                <Size X="750.0000" Y="580.0000" />
                <AnchorPoint ScaleX="0.4965" ScaleY="1.0000" />
                <Position X="372.6144" Y="1287.7876" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4968" Y="0.9654" />
                <PreSize X="1.0000" Y="0.4348" />
                <FileData Type="Normal" Path="assets/enemyrect.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="ownRect" ActionTag="-240575767" Tag="32" IconVisible="False" LeftMargin="0.3638" RightMargin="-0.3638" TopMargin="625.9124" BottomMargin="128.0876" ctype="SpriteObjectData">
                <Size X="750.0000" Y="580.0000" />
                <AnchorPoint ScaleX="0.5000" />
                <Position X="375.3638" Y="128.0876" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5005" Y="0.0960" />
                <PreSize X="1.0000" Y="0.4348" />
                <FileData Type="Normal" Path="assets/ownrect.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="returnBtn" ActionTag="653611871" Tag="15" IconVisible="False" LeftMargin="18.3135" RightMargin="675.6865" TopMargin="22.6863" BottomMargin="1248.3137" ctype="SpriteObjectData">
            <Size X="56.0000" Y="63.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="46.3135" Y="1279.8137" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.0618" Y="0.9594" />
            <PreSize X="0.0747" Y="0.0472" />
            <FileData Type="Normal" Path="assets/returnBtn.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="topGroup" ActionTag="1279103855" Tag="21" IconVisible="False" LeftMargin="205.0000" RightMargin="205.0000" BottomMargin="1234.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="340.0000" Y="100.0000" />
            <Children>
              <AbstractNodeData Name="topbg_5" ActionTag="-1692433080" Tag="16" IconVisible="False" LeftMargin="-11.1092" RightMargin="-31.8908" TopMargin="1.5261" BottomMargin="-2.5261" ctype="SpriteObjectData">
                <Size X="383.0000" Y="101.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                <Position X="180.3908" Y="98.4739" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5306" Y="0.9847" />
                <PreSize X="1.1265" Y="1.0100" />
                <FileData Type="Normal" Path="assets/topbg.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="font2_6" ActionTag="560409714" Tag="17" IconVisible="False" LeftMargin="29.3258" RightMargin="214.6742" TopMargin="15.4391" BottomMargin="66.5609" ctype="SpriteObjectData">
                <Size X="96.0000" Y="18.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="77.3258" Y="75.5609" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2274" Y="0.7556" />
                <PreSize X="0.2824" Y="0.1800" />
                <FileData Type="Normal" Path="assets/font2.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="font_7" ActionTag="-322826895" Tag="18" IconVisible="False" LeftMargin="217.6827" RightMargin="4.3173" TopMargin="14.4384" BottomMargin="67.5616" ctype="SpriteObjectData">
                <Size X="118.0000" Y="18.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="276.6827" Y="76.5616" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.8138" Y="0.7656" />
                <PreSize X="0.3471" Y="0.1800" />
                <FileData Type="Normal" Path="assets/font.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="ownScore" ActionTag="949750206" Tag="19" IconVisible="False" LeftMargin="59.9347" RightMargin="246.0653" TopMargin="46.2219" BottomMargin="30.7781" FontSize="20" LabelText="100" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="34.0000" Y="23.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="76.9347" Y="42.2781" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2263" Y="0.4228" />
                <PreSize X="0.1000" Y="0.2300" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="enemyScore" ActionTag="182746087" Tag="20" IconVisible="False" LeftMargin="255.8830" RightMargin="50.1170" TopMargin="47.0273" BottomMargin="29.9727" FontSize="20" LabelText="100" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="34.0000" Y="23.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="272.8830" Y="41.4727" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.8026" Y="0.4147" />
                <PreSize X="0.1000" Y="0.2300" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
            <Position X="375.0000" Y="1334.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="1.0000" />
            <PreSize X="0.4533" Y="0.0750" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="hpbg" ActionTag="-1934413237" Tag="22" IconVisible="False" LeftMargin="14.0000" RightMargin="14.0000" TopMargin="1270.1626" BottomMargin="4.8374" ctype="SpriteObjectData">
            <Size X="722.0000" Y="59.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="375.0000" Y="34.3374" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.0257" />
            <PreSize X="0.9627" Y="0.0442" />
            <FileData Type="Normal" Path="assets/hpbg.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="hpBar" ActionTag="2121307027" Tag="25" IconVisible="False" LeftMargin="76.3470" RightMargin="55.6530" TopMargin="1297.5884" BottomMargin="19.4116" ProgressInfo="100" ctype="LoadingBarObjectData">
            <Size X="618.0000" Y="17.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="385.3470" Y="27.9116" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5138" Y="0.0209" />
            <PreSize X="0.8240" Y="0.0127" />
            <ImageFileData Type="Normal" Path="assets/hpbar.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="hero1" CanEdit="False" ActionTag="1295461217" Tag="26" IconVisible="False" LeftMargin="618.6163" RightMargin="-3.6163" TopMargin="589.1260" BottomMargin="586.8740" ctype="SpriteObjectData">
            <Size X="135.0000" Y="158.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="686.1163" Y="665.8740" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.9148" Y="0.4992" />
            <PreSize X="0.1800" Y="0.1184" />
            <FileData Type="Normal" Path="assets/hero2.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="hero2" CanEdit="False" ActionTag="-1700475750" Tag="27" IconVisible="False" LeftMargin="619.3049" RightMargin="-4.3049" TopMargin="746.7094" BottomMargin="429.2906" ctype="SpriteObjectData">
            <Size X="135.0000" Y="158.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="686.8049" Y="508.2906" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.9157" Y="0.3810" />
            <PreSize X="0.1800" Y="0.1184" />
            <FileData Type="Normal" Path="assets/hero3.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="hero3" CanEdit="False" ActionTag="835198709" Tag="30" IconVisible="False" LeftMargin="618.2333" RightMargin="-3.2333" TopMargin="905.1813" BottomMargin="270.8187" ctype="SpriteObjectData">
            <Size X="135.0000" Y="158.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="685.7333" Y="349.8187" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.9143" Y="0.2622" />
            <PreSize X="0.1800" Y="0.1184" />
            <FileData Type="Normal" Path="assets/hero1.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="helpImg" ActionTag="-1666437148" Tag="34" IconVisible="False" ctype="SpriteObjectData">
            <Size X="750.0000" Y="1334.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="375.0000" Y="667.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <FileData Type="Normal" Path="assets/helpImg.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="hpShow" CanEdit="False" ActionTag="-1712330744" Tag="59" IconVisible="False" ctype="SpriteObjectData">
            <Size X="750.0000" Y="1334.0000" />
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="1.0000" Y="1.0000" />
            <FileData Type="Normal" Path="assets/be_shot.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="resultPanel" ActionTag="-1854181925" Tag="60" IconVisible="False" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="750.0000" Y="1334.0000" />
            <Children>
              <AbstractNodeData Name="continueBtn" ActionTag="758953948" Tag="63" IconVisible="False" LeftMargin="293.1146" RightMargin="277.8854" TopMargin="825.6163" BottomMargin="447.3837" ctype="SpriteObjectData">
                <Size X="179.0000" Y="61.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="382.6146" Y="477.8837" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5102" Y="0.3582" />
                <PreSize X="0.2387" Y="0.0457" />
                <FileData Type="Normal" Path="assets/continueBtn.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="scoreLab" ActionTag="-1907033436" Tag="66" IconVisible="False" LeftMargin="351.6507" RightMargin="347.3493" TopMargin="667.5897" BottomMargin="632.4103" FontSize="30" LabelText="000" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="51.0000" Y="34.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="377.1507" Y="649.4103" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5029" Y="0.4868" />
                <PreSize X="0.0680" Y="0.0255" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="scoreFont" ActionTag="1196538101" Tag="65" IconVisible="False" LeftMargin="299.6122" RightMargin="288.3878" TopMargin="588.5956" BottomMargin="704.4044" ctype="SpriteObjectData">
                <Size X="162.0000" Y="41.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="380.6122" Y="724.9044" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5075" Y="0.5434" />
                <PreSize X="0.2160" Y="0.0307" />
                <FileData Type="Normal" Path="assets/scoreLab.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="loseicon" ActionTag="719602956" Tag="64" IconVisible="False" LeftMargin="101.4210" RightMargin="89.5790" TopMargin="209.1477" BottomMargin="832.8523" ctype="SpriteObjectData">
                <Size X="559.0000" Y="292.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="380.9210" Y="978.8523" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5079" Y="0.7338" />
                <PreSize X="0.7453" Y="0.2189" />
                <FileData Type="Normal" Path="assets/loseicon.png" Plist="" />
                <BlendFunc Src="1" Dst="771" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="0" G="0" B="0" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>