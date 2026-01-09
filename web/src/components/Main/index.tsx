import { Tabs } from '@mantine/core';
import Editor from '../Editor';
import Text from '../Text';
import Icons from '../Icons';
import Effects from '../Effects';
import styles from './index.module.sass';
import Settings from '../Settings';

const Main = () => {
  return (
    <Tabs defaultValue="editor" className={styles.wrapper}>
      <Tabs.Panel value="editor" p="xs">
        <Editor></Editor>
      </Tabs.Panel>
      
      <Tabs.Panel value="text" p="xs">
        <Text></Text>
      </Tabs.Panel>
      
      <Tabs.Panel value="icons" p="xs">
        <Icons></Icons>
      </Tabs.Panel>
      
      <Tabs.Panel value="effects" p="xs">
        <Effects></Effects>
      </Tabs.Panel>

      <Tabs.Panel value="settings" p="xs">
        <Settings></Settings>
      </Tabs.Panel>

      <Tabs.List justify="space-between" className={styles.menu}>
        <Tabs.Tab value="editor">Editor</Tabs.Tab>
        <Tabs.Tab value="text">Text</Tabs.Tab>
        <Tabs.Tab value="icons">Icons</Tabs.Tab>
        <Tabs.Tab value="effects">Effects</Tabs.Tab>
        <Tabs.Tab value="settings">Settings</Tabs.Tab>
      </Tabs.List>
    </Tabs>
  );
};

export default Main;