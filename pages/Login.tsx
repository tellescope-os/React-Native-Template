import React from "react"
import { SafeAreaView } from "react-native"
import { TELLESCOPE_TEST_URL } from "@env"

import { 
  EnduserLogin,

  Flex,
  Paper,
  Typography,
} from "@tellescope/react-components"

export const Login = () => (
  <SafeAreaView style={{ minHeight: '100%' }}>  
  <Flex flex={1} alignContent='center'>
    <Paper flex style={{ margin: 15, borderRadius: 7, padding: 15 }}>
      <EnduserLogin/>
      {__DEV__ && <Typography>host: {TELLESCOPE_TEST_URL ?? 'default'}</Typography>}
    </Paper>
  </Flex>
  </SafeAreaView>
)